<?php

/**
 * @file
 * Contains FeedsParser and related classes.
 */

/**
 * A result of a parsing stage.
 */
class FeedsParserResult extends FeedsResult {
  public $title;
  public $description;
  public $link;
  public $items;
  public $current_item;

  /**
   * Constructor.
   */
  public function __construct($items = array()) {
    $this->title = '';
    $this->description = '';
    $this->link = '';
    $this->items = $items;
  }

  /**
   * @todo Move to a nextItem() based approach, not consuming the item array.
   *   Can only be done once we don't cache the entire batch object between page
   *   loads for batching anymore.
   *
   * @return
   *   Next available item or NULL if there is none. Every returned item is
   *   removed from the internal array.
   */
  public function shiftItem() {
    $this->current_item = array_shift($this->items);
    return $this->current_item;
  }

  /**
   * @return
   *   Current result item.
   */
  public function currentItem() {
    return empty($this->current_item) ? NULL : $this->current_item;
  }

}

/**
 * Abstract class, defines interface for parsers.
 */
abstract class FeedsParser extends FeedsPlugin {

  /**
   * Implements FeedsPlugin::pluginType().
   */
  public function pluginType() {
    return 'parser';
  }

  /**
   * Parse content fetched by fetcher.
   *
   * Extending classes must implement this method.
   *
   * @param FeedsSource $source
   *   Source information.
   * @param $fetcher_result
   *   FeedsFetcherResult returned by fetcher.
   */
  abstract public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result);

  /**
   * Clear all caches for results for given source.
   *
   * @param FeedsSource $source
   *   Source information for this expiry. Implementers can choose to only clear
   *   caches pertaining to this source.
   */
  public function clear(FeedsSource $source) {}

  /**
   * Declare the possible mapping sources that this parser produces.
   *
   * @ingroup mappingapi
   *
   * @return
   *   An array of mapping sources, or FALSE if the sources can be defined by
   *   typing a value in a text field.
   *   @code
   *   array(
   *     'title' => t('Title'),
   *     'created' => t('Published date'),
   *     'url' => t('Feed item URL'),
   *     'guid' => t('Feed item GUID'),
   *   )
   *   @endcode
   */
  public function getMappingSources() {
    self::loadMappers();
    $sources = array();
    $content_type = feeds_importer($this->id)->config['content_type'];
    backdrop_alter('feeds_parser_sources', $sources, $content_type);
    if (!feeds_importer($this->id)->config['content_type']) {
      return $sources;
    }
    $sources['parent:uid'] = array(
      'name' => t('Feed node: User ID'),
      'description' => t('The feed node author uid.'),
    );
    $sources['parent:nid'] = array(
      'name' => t('Feed node: Node ID'),
      'description' => t('The feed node nid.'),
    );
    return $sources;
  }

  /**
   * Get list of mapped sources.
   *
   * @return array
   *   List of mapped source names in an array.
   */
  public function getMappingSourceList() {
    $mappings = feeds_importer($this->id)->processor->config['mappings'];
    $sources = array();
    foreach ($mappings as $mapping) {
      $sources[] = $mapping['source'];
    }
    return $sources;
  }

  /**
   * Get an element identified by $element_key of the given item.
   * The element key corresponds to the values in the array returned by
   * FeedsParser::getMappingSources().
   *
   * This method is invoked from FeedsProcessor::map() when a concrete item is
   * processed.
   *
   * @ingroup mappingapi
   *
   * @param $batch
   *   FeedsImportBatch object containing the sources to be mapped from.
   * @param $element_key
   *   The key identifying the element that should be retrieved from $source
   *
   * @return
   *   The source element from $item identified by $element_key.
   *
   * @see FeedsProcessor::map()
   * @see FeedsCSVParser::getSourceElement()
   */
  public function getSourceElement(FeedsSource $source, FeedsParserResult $result, $element_key) {

    switch ($element_key) {

      case 'parent:uid':
        if ($source->feed_nid && $node = node_load($source->feed_nid)) {
          return $node->uid;
        }
        break;

      case 'parent:nid':
        return $source->feed_nid;
    }

    $item = $result->currentItem();
    return isset($item[$element_key]) ? $item[$element_key] : '';
  }

  /**
   * Returns if the parsed result can have a title.
   *
   * Parser classes should override this method in case they support a source
   * title.
   *
   * @return bool
   *   TRUE if the parsed result can have a title.
   *   FALSE otherwise.
   */
  public function providesSourceTitle() {
    return FALSE;
  }

}

/**
 * Defines an element of a parsed result. Such an element can be a simple type,
 * a complex type (derived from FeedsElement) or an array of either.
 *
 * @see FeedsEnclosure
 */
class FeedsElement {

  /**
   * The standard value of this element. This value can contain be a simple
   * type, a FeedsElement or an array of either.
   */
  protected $value;

  /**
   * Constructor.
   */
  public function __construct($value) {
    $this->value = $value;
  }

  /**
   * @todo Make value public and deprecate use of getValue().
   *
   * @return
   *   Value of this FeedsElement represented as a scalar.
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * Magic method __toString() for printing and string conversion of this
   * object.
   *
   * @return
   *   A string representation of this element.
   */
  public function __toString() {
    if (is_array($this->value)) {
      return 'Array';
    }
    if (is_object($this->value)) {
      return 'Object';
    }
    return (string) $this->getValue();
  }

}

/**
 * Encapsulates a taxonomy style term object.
 *
 * Objects of this class can be turned into a taxonomy term style arrays by
 * casting them.
 *
 * @code
 *   $term_object = new FeedsTermElement($term_array);
 *   $term_array = (array)$term_object;
 * @endcode
 */
class FeedsTermElement extends FeedsElement {
  public $tid, $vid, $name;

  /**
   * @param $term
   *   An array or a stdClass object that is a Backdrop taxonomy term.
   */
  public function __construct($term) {
    if (is_array($term)) {
      parent::__construct($term['name']);
      foreach ($this as $key => $value) {
        $this->$key = isset($term[$key]) ? $term[$key] : NULL;
      }
    }
    elseif (is_object($term)) {
      parent::__construct($term->name);
      foreach ($this as $key => $value) {
        $this->$key = isset($term->$key) ? $term->$key : NULL;
      }
    }
  }

  /**
   * Use $name as $value.
   */
  public function getValue() {
    return $this->name;
  }

}

/**
 * A geo term element.
 */
class FeedsGeoTermElement extends FeedsTermElement {
  public $lat, $lon, $bound_top, $bound_right, $bound_bottom, $bound_left, $geometry;

  /**
   * @param $term
   *   An array or a stdClass object that is a Backdrop taxonomy term. Can include
   *   geo extensions.
   */
  public function __construct($term) {
    parent::__construct($term);
  }

}

/**
 * Enclosure element, can be part of the result array.
 */
class FeedsEnclosure extends FeedsElement {

  /**
   * The mime type of the enclosure.
   *
   * @param string
   */
  protected $mime_type;

  /**
   * The default list of allowed extensions.
   *
   * @param string
   */
  protected $allowedExtensions = 'jpg jpeg gif png txt doc xls pdf ppt pps odt ods odp';

  /**
   * The sanitized local file name.
   *
   * @var string
   */
  protected $safeFilename;

  /**
   * Constructor, requires MIME type.
   *
   * @param $value
   *   A path to a local file or a URL to a remote document.
   * @param $mimetype
   *   The mime type of the resource.
   */
  public function __construct($value, $mime_type) {
    parent::__construct($value);
    $this->mime_type = $mime_type;
  }

  /**
   * @return
   *   MIME type of return value of getValue().
   */
  public function getMIMEType() {
    return $this->mime_type;
  }

  /**
   * Sets the list of allowed extensions.
   *
   * @param string $extensions
   *   The list of allowed extensions separated by a space.
   */
  public function setAllowedExtensions($extensions) {
    // Normalize whitespace so that empty extensions are not allowed.
    $this->allowedExtensions = backdrop_strtolower(trim(preg_replace('/\s+/', ' ', $extensions)));
  }

  /**
   * Use this method instead of FeedsElement::getValue() when fetching the file
   * from the URL.
   *
   * @return
   *   Value with encoded space characters to safely fetch the file from the URL.
   *
   * @see FeedsElement::getValue()
   */
  public function getUrlEncodedValue() {
    return str_replace(' ', '%20', $this->getValue());
  }

  /**
   * Returns the full path to the file URI with a safe file name.
   *
   * @return string
   *   The safe file URI.
   *
   * @throws RuntimeException
   *   Thrown if the file extension is invalid.
   */
  public function getSanitizedUri() {
    return backdrop_dirname($this->getValue()) . '/' . $this->getSafeFilename();
  }

  /**
   * Returns the file name transformed for better local saving.
   *
   * @return string
   *   Value with space characters changed to underscores.
   *
   * @throws RuntimeException
   *   Thrown if the file extension is invalid.
   */
  public function getLocalValue() {
    return str_replace(' ', '_', $this->getSafeFilename());
  }

  /**
   * Returns the safe file name.
   *
   * @return string
   *   A filename that is safe to save to the filesystem.
   *
   * @throws RuntimeException
   *   Thrown if the file extension is invalid.
   */
  protected function getSafeFilename() {
    if (isset($this->safeFilename)) {
      return $this->safeFilename;
    }

    // Strip any query string or fragment from file name.
    list($filename) = explode('?', $this->getValue());
    list($filename) = explode('#', $filename);

    $filename = rawurldecode(backdrop_basename($filename));

    // Remove leading and trailing whitespace and periods.
    $filename = trim($filename, " \t\n\r\0\x0B.");

    if (strpos($filename, '.') === FALSE) {
      $extension = FALSE;
    }
    else {
      $extension = backdrop_strtolower(substr($filename, strrpos($filename, '.') + 1));
    }

    if (!$extension || !in_array($extension, explode(' ', $this->allowedExtensions), TRUE)) {
      throw new RuntimeException(t('The file @file has an invalid extension.', array('@file' => $filename)));
    }

    $this->safeFilename = file_munge_filename($filename, $this->allowedExtensions, FALSE);

    return $this->safeFilename;
  }

  /**
   * Downloads the content from the file URL.
   *
   * @return string
   *   The content of the referenced resource.
   *
   * @throws FeedsHTTPRequestException
   *   In case the result code of the HTTP request is not in the 2xx series.
   */
  public function getContent() {
    feeds_include_library('http_request.inc', 'http_request');
    $result = feeds_http_request($this->getUrlEncodedValue());
    http_request_check_result($this->getUrlEncodedValue(), $result);
    return $result->data;
  }

  /**
   * Get a Backdrop file object of the enclosed resource, download if necessary.
   *
   * @param string $destination
   *   The path or uri specifying the target directory in which the file is
   *   expected. Don't use trailing slashes unless it's a streamwrapper scheme.
   * @param int $replace
   *   Replace behavior when the destination file already exists.
   *
   * @see file_save_data()
   *
   * @return object|false
   *   A Backdrop temporary file object of the enclosed resource or FALSE if the
   *   value is empty.
   *
   * @throws Exception
   *   If file object could not be created.
   */
  public function getFile($destination, $replace = FILE_EXISTS_RENAME) {
    $file = FALSE;
    if ($this->getValue()) {
      // Prepare destination directory.
      file_prepare_directory($destination, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY);
      // Copy or save file depending on whether it is remote or local.
      if (backdrop_realpath($this->getSanitizedUri())) {
        $file = new File();
        $file->uid = 0;
        $file->uri = $this->getSanitizedUri();
        $file->filemime = $this->getMIMEType();
        $file->filename = $this->getSafeFilename();

        if (backdrop_dirname($file->uri) !== $destination) {
          $file = file_copy($file, $destination, $replace);
        }
        else {
          // If file is not to be copied, check whether file already exists,
          // as file_save() won't do that for us (compare file_copy() and
          // file_save())
          $existing_files = file_load_multiple(array(), array('uri' => $file->uri));
          if (count($existing_files)) {
            $existing = reset($existing_files);
            if ($replace == FEEDS_FILE_EXISTS_SKIP) {
              return $existing;
            }
            $file->fid = $existing->fid;
            $file->filename = $existing->filename;
          }
          file_save($file);
        }
      }
      else {
        if (file_uri_target($destination)) {
          $destination = trim($destination, '/') . '/';
        }
        try {
          $filename = $this->getLocalValue();

          if (module_exists('transliteration')) {
            require_once backdrop_get_path('module', 'transliteration') . '/transliteration.inc';
            $filename = transliteration_clean_filename($filename);
          }

          $file = file_save_data($this->getContent(), $destination . $filename, $replace);
        }
        catch (Exception $e) {
          watchdog_exception('Feeds', $e, nl2br(check_plain($e)));
        }
      }

      // We couldn't make sense of this enclosure, throw an exception.
      if (!$file) {
        throw new Exception(t('Invalid enclosure %enclosure', array('%enclosure' => $this->getValue())));
      }

      return $file;
    }
  }

}

/**
 * Converts to UNIX time.
 *
 * @param string|BackdropDateTime $date
 *   A date that is either a string, a BackdropDateTime object, or a UNIX timestamp.
 * @param $default_value
 *   A default UNIX timestamp to return if $date could not be parsed.
 *
 * @return
 *   $date as UNIX time if conversion was successful, $default_value otherwise.
 */
function feeds_to_unixtime($date, $default_value) {
  if (is_numeric($date)) {
    return $date;
  }

  if (is_string($date) || is_object($date) && method_exists($date, '__toString')) {
    if ($date_object = date_create(trim($date))) {
      return $date_object->format('U');
    }
  }

  return $default_value;
}
