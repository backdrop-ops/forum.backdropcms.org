<?php
/**
 * @file
 * Functions to aid in presenting database results as a set of pages.
 */

/**
 * Returns the current page being requested for display within a pager.
 *
 * @param $element
 *  An optional integer to distinguish between multiple pagers on one page.
 *
 * @return
 *  The number of the current requested page, within the pager represented by
 *  $element. This is determined from the URL query parameter $_GET['page'], or
 *  0 by default. Note that this number may differ from the actual page being
 *  displayed. For example, if a search for "example text" brings up three
 *  pages of results, but a users visits search/node/example+text?page=10, this
 *  function will return 10, even though the default pager implementation
 *  adjusts for this and still displays the third page of search results at
 *  that URL.
 *
 * @see pager_default_initialize()
 */
function pager_find_page($element = 0) {
  $page = isset($_GET['page']) ? $_GET['page'] : '';
  $page_array = explode(',', $page);
  if (!isset($page_array[$element])) {
    $page_array[$element] = 0;
  }
  return (int) $page_array[$element];
}

/**
 * Initializes a pager for theme('pager').
 *
 * This function sets up the necessary global variables so that future calls
 * to theme('pager') will render a pager that correctly corresponds to the
 * items being displayed.
 *
 * If the items being displayed result from a database query performed using
 * Backdrop's database API, and if you have control over the construction of the
 * database query, you do not need to call this function directly; instead, you
 * can extend the query object with the 'PagerDefault' extender before executing
 * it. For example:
 * @code
 *   $query = db_select('some_table')->extend('PagerDefault');
 * @endcode
 *
 * However, if you are using a different method for generating the items to be
 * paged through, then you should call this function in preparation.
 *
 * The following example shows how this function can be used in a page callback
 * that invokes an external datastore with an SQL-like syntax:
 * @code
 *   // First find the total number of items and initialize the pager.
 *   $where = "status = 1";
 *   $total = my_module_select("SELECT COUNT(*) FROM data " . $where)->result();
 *   $num_per_page = config_get('my_module.settings', 'num_per_page');
 *   $page = pager_default_initialize($total, $num_per_page);
 *
 *   // Next, retrieve and display the items for the current page.
 *   $offset = $num_per_page * $page;
 *   $result = my_module_select("SELECT * FROM data " . $where . " LIMIT %d, %d", $offset, $num_per_page)->fetchAll();
 *   $output = theme('my_module_results', array('result' => $result));
 *
 *   // Finally, display the pager controls, and return.
 *   $output .= theme('pager');
 *   return $output;
 * @endcode
 *
 * A second example involves a page callback that invokes an external search
 * service where the total number of matching results is provided as part of
 * the returned set (so that we do not need a separate query in order to obtain
 * this information). Here, we call pager_find_page() to calculate the desired
 * offset before the search is invoked:
 * @code
 *   // Perform the query, using the requested offset from pager_find_page().
 *   // This comes from a URL parameter, so here we are assuming that the URL
 *   // parameter corresponds to an actual page of results that will exist
 *   // within the set.
 *   $page = pager_find_page();
 *   $num_per_page = config_get('my_module.settings', 'num_per_page');
 *   $offset = $num_per_page * $page;
 *   $result = my_module_remote_search($keywords, $offset, $num_per_page);
 *
 *   // Now that we have the total number of results, initialize the pager.
 *   pager_default_initialize($result->total, $num_per_page);
 *
 *   // Display the search results.
 *   $output = theme('search_results', array('results' => $result->data, 'type' => 'remote'));
 *
 *   // Finally, display the pager controls, and return.
 *   $output .= theme('pager');
 *   return $output;
 * @endcode
 *
 * @param $total
 *  The total number of items to be paged.
 * @param $limit
 *  The number of items the calling code will display per page.
 * @param $element
 *  An optional integer to distinguish between multiple pagers on one page.
 *
 * @return
 *   The number of the current page, within the pager represented by $element.
 *   This is determined from the URL query parameter $_GET['page'], or 0 by
 *   default. However, if a page that does not correspond to the actual range
 *   of the result set was requested, this function will return the closest
 *   page actually within the result set.
 */
function pager_default_initialize($total, $limit, $element = 0) {
  global $pager_page_array, $pager_total, $pager_total_items, $pager_limits;

  $page = pager_find_page($element);

  // We calculate the total of pages as ceil(items / limit).
  $pager_total_items[$element] = $total;
  $pager_total[$element] = ceil($pager_total_items[$element] / $limit);
  $pager_page_array[$element] = max(0, min($page, ((int) $pager_total[$element]) - 1));
  $pager_limits[$element] = $limit;
  return $pager_page_array[$element];
}

/**
 * Compose a URL query parameter array for pager links.
 *
 * @return
 *   A URL query parameter array that consists of all components of the current
 *   page request except for those pertaining to paging.
 */
function pager_get_query_parameters() {
  $query = &backdrop_static(__FUNCTION__);
  if (!isset($query)) {
    $query = backdrop_get_query_parameters($_GET, array('q', 'page'));
  }
  return $query;
}

/**
 * Returns HTML for a query pager.
 *
 * Menu callbacks that display paged query results should call theme('pager') to
 * retrieve a pager control so that users can view other results. Format a list
 * of nearby pages with additional query results.
 *
 * @param $variables
 *   An associative array containing:
 *   - tags: An array of labels for the controls in the pager.
 *   - element: An optional integer to distinguish between multiple pagers on
 *     one page.
 *   - parameters: An associative array of query string parameters to append to
 *     the pager links.
 *   - quantity: The number of pages in the list.
 *
 * @ingroup themeable
 */
function theme_pager($variables) {
  $tags = $variables['tags'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $quantity = empty($variables['quantity']) ? 0 : (int) $variables['quantity'];
  global $pager_page_array, $pager_total;

  // Return if there is no pager to be rendered.
  if (!isset($pager_page_array[$element]) || empty($pager_total)) {
    return '';
  }

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // first is the first page listed by this pager piece (re quantity)
  $pager_first = $pager_current - $pager_middle + 1;
  // last is the last page listed by this pager piece (re quantity)
  $pager_last = $pager_current + $quantity - $pager_middle;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }
  // End of generation loop preparation.

  $li_first = theme('pager_first', array('text' => (isset($tags[0]) ? $tags[0] : t('« first')), 'element' => $element, 'parameters' => $parameters));
  $li_previous = theme('pager_previous', array('text' => (isset($tags[1]) ? $tags[1] : t('‹ previous')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_next = theme('pager_next', array('text' => (isset($tags[3]) ? $tags[3] : t('next ›')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_last = theme('pager_last', array('text' => (isset($tags[4]) ? $tags[4] : t('last »')), 'element' => $element, 'parameters' => $parameters));

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'class' => array('pager-first'),
        'data' => $li_first,
      );
    }
    if ($li_previous) {
      $items[] = array(
        'class' => array('pager-previous'),
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      if ($i > 1) {
        $items[] = array(
          'class' => array('pager-ellipsis'),
          'data' => '…',
        );
      }
      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($pager_current - $i), 'parameters' => $parameters)),
          );
        }
        if ($i == $pager_current) {
          $items[] = array(
            'class' => array('pager-current'),
            'data' => $i,
          );
        }
        if ($i > $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $pager_current), 'parameters' => $parameters)),
          );
        }
      }
      if ($i < $pager_max) {
        $items[] = array(
          'class' => array('pager-ellipsis'),
          'data' => '…',
        );
      }
    }
    // End generation.
    if ($li_next) {
      $items[] = array(
        'class' => array('pager-next'),
        'data' => $li_next,
      );
    }
    if ($li_last) {
      $items[] = array(
        'class' => array('pager-last'),
        'data' => $li_last,
      );
    }
    return '<h2 class="element-invisible">' . t('Pages') . '</h2>' . theme('item_list', array(
      'items' => $items,
      'attributes' => array('class' => array('pager')),
    ));
  }

  // Single page of contents, no pager needed.
  return '';
}


/**
 * @defgroup pagerpieces Pager pieces
 * @{
 * Theme functions for customizing pager elements.
 *
 * Note that you should NOT modify this file to customize your pager.
 */

/**
 * Returns HTML for the "first page" link in a query pager.
 *
 * @param $variables
 *   An associative array containing:
 *   - text: The name (or image) of the link.
 *   - element: An optional integer to distinguish between multiple pagers on
 *     one page.
 *   - parameters: An associative array of query string parameters to append to
 *     the pager links.
 *
 * @ingroup themeable
 */
function theme_pager_first($variables) {
  $text = $variables['text'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  global $pager_page_array;
  $output = '';

  // If we are anywhere but the first page
  if ($pager_page_array[$element] > 0) {
    $output = theme('pager_link', array('text' => $text, 'page_new' => pager_load_array(0, $element, $pager_page_array), 'element' => $element, 'parameters' => $parameters));
  }

  return $output;
}

/**
 * Returns HTML for the "previous page" link in a query pager.
 *
 * @param $variables
 *   An associative array containing:
 *   - text: The name (or image) of the link.
 *   - element: An optional integer to distinguish between multiple pagers on
 *     one page.
 *   - interval: The number of pages to move backward when the link is clicked.
 *   - parameters: An associative array of query string parameters to append to
 *     the pager links.
 *
 * @ingroup themeable
 */
function theme_pager_previous($variables) {
  $text = $variables['text'];
  $element = $variables['element'];
  $interval = $variables['interval'];
  $parameters = $variables['parameters'];
  global $pager_page_array;
  $output = '';

  // If we are anywhere but the first page
  if ($pager_page_array[$element] > 0) {
    $page_new = pager_load_array($pager_page_array[$element] - $interval, $element, $pager_page_array);

    // If the previous page is the first page, mark the link as such.
    if ($page_new[$element] == 0) {
      $output = theme('pager_first', array('text' => $text, 'element' => $element, 'parameters' => $parameters));
    }
    // The previous page is not the first page.
    else {
      $output = theme('pager_link', array('text' => $text, 'page_new' => $page_new, 'element' => $element, 'parameters' => $parameters));
    }
  }

  return $output;
}

/**
 * Returns HTML for the "next page" link in a query pager.
 *
 * @param $variables
 *   An associative array containing:
 *   - text: The name (or image) of the link.
 *   - element: An optional integer to distinguish between multiple pagers on
 *     one page.
 *   - interval: The number of pages to move forward when the link is clicked.
 *   - parameters: An associative array of query string parameters to append to
 *     the pager links.
 *
 * @ingroup themeable
 */
function theme_pager_next($variables) {
  $text = $variables['text'];
  $element = $variables['element'];
  $interval = $variables['interval'];
  $parameters = $variables['parameters'];
  global $pager_page_array, $pager_total;
  $output = '';

  // If we are anywhere but the last page
  if ($pager_page_array[$element] < ($pager_total[$element] - 1)) {
    $page_new = pager_load_array($pager_page_array[$element] + $interval, $element, $pager_page_array);
    // If the next page is the last page, mark the link as such.
    if ($page_new[$element] == ($pager_total[$element] - 1)) {
      $output = theme('pager_last', array('text' => $text, 'element' => $element, 'parameters' => $parameters));
    }
    // The next page is not the last page.
    else {
      $output = theme('pager_link', array('text' => $text, 'page_new' => $page_new, 'element' => $element, 'parameters' => $parameters));
    }
  }

  return $output;
}

/**
 * Returns HTML for the "last page" link in query pager.
 *
 * @param $variables
 *   An associative array containing:
 *   - text: The name (or image) of the link.
 *   - element: An optional integer to distinguish between multiple pagers on
 *     one page.
 *   - parameters: An associative array of query string parameters to append to
 *     the pager links.
 *
 * @ingroup themeable
 */
function theme_pager_last($variables) {
  $text = $variables['text'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  global $pager_page_array, $pager_total;
  $output = '';

  // If we are anywhere but the last page
  if ($pager_page_array[$element] < ($pager_total[$element] - 1)) {
    $output = theme('pager_link', array('text' => $text, 'page_new' => pager_load_array($pager_total[$element] - 1, $element, $pager_page_array), 'element' => $element, 'parameters' => $parameters));
  }

  return $output;
}


/**
 * Returns HTML for a link to a specific query result page.
 *
 * @param $variables
 *   An associative array containing:
 *   - text: The link text. Also used to figure out the title attribute of the
 *     link, if it is not provided in $variables['attributes']['title']; in
 *     this case, $variables['text'] must be one of the standard pager link
 *     text strings that would be generated by the pager theme functions, such
 *     as a number or t('« first').
 *   - page_new: The first result to display on the linked page.
 *   - element: An optional integer to distinguish between multiple pagers on
 *     one page.
 *   - parameters: An associative array of query string parameters to append to
 *     the pager link.
 *   - attributes: An associative array of HTML attributes to apply to the
 *     pager link.
 *
 * @see theme_pager()
 *
 * @ingroup themeable
 */
function theme_pager_link($variables) {
  $text = $variables['text'];
  $page_new = $variables['page_new'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $attributes = $variables['attributes'];

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  if ($new_page = implode(',', pager_load_array($page_new[$element], $element, explode(',', $page)))) {
    $parameters['page'] = $new_page;
  }

  $query = array();
  if (count($parameters)) {
    $query = backdrop_get_query_parameters($parameters, array());
  }
  if ($query_pager = pager_get_query_parameters()) {
    $query = array_merge($query, $query_pager);
  }

  // Set each pager link title
  if (!isset($attributes['title'])) {
    static $titles = NULL;
    if (!isset($titles)) {
      $titles = array(
        t('« first') => t('Go to first page'),
        t('‹ previous') => t('Go to previous page'),
        t('next ›') => t('Go to next page'),
        t('last »') => t('Go to last page'),
      );
    }
    if (isset($titles[$text])) {
      $attributes['title'] = $titles[$text];
    }
    elseif (is_numeric($text)) {
      $attributes['title'] = t('Go to page @number', array('@number' => $text));
    }
  }

  // @todo l() cannot be used here, since it adds an 'active' class based on the
  //   path only (which is always the current path for pager links). Apparently,
  //   none of the pager links is active at any time - but it should still be
  //   possible to use l() here.
  // @see http://drupal.org/node/1410574
  $attributes['href'] = url($_GET['q'], array('query' => $query));
  return '<a' . backdrop_attributes($attributes) . '>' . check_plain($text) . '</a>';
}

/**
 * @} End of "Pager pieces".
 */

/**
 * Helper function
 *
 * Copies $old_array to $new_array and sets $new_array[$element] = $value
 * Fills in $new_array[0 .. $element - 1] = 0
 */
function pager_load_array($value, $element, $old_array) {
  $new_array = $old_array;
  // Look for empty elements.
  for ($i = 0; $i < $element; $i++) {
    if (empty($new_array[$i])) {
      // Load found empty element with 0.
      $new_array[$i] = 0;
    }
  }
  // Update the changed element.
  $new_array[$element] = (int) $value;
  return $new_array;
}
