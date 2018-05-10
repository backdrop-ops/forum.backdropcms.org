<?php
/**
 * @file
 * Defines the Similar entries node ID argument.
 */

/**
 * Defines the similar entries View node ID argument.
 */
class similar_handler_argument_nid extends views_handler_argument_numeric {

  /**
   * Defines default values for argument settings.
   */
  public function option_definition() {
    $common_words = array('-not', '-and', '-a', '-or', '-the', '-but', '-therefore',
      '-because', '-can\'t', '-can', '-for', '-until', '-the', '-of', '-and', '-to',
      '-in', '-is', '-you', '-that', '-it', '-he', '-was', '-on', '-are', '-as', '-with',
      '-his', '-they', '-I', '-at', '-be', '-this', '-have', '-from', '-one', '-had',
      '-by', '-word', '-but', '-not', '-what', '-all', '-were', '-we', '-when', '-your',
      '-said', '-there', '-use', '-an', '-each', '-which', '-she', '-do', '-how',
      '-their', '-if', '-will', '-up', '-other', '-about', '-out', '-many', '-then',
      '-them', '-these', '-so', '-some', '-her', '-would', '-make', '-like', '-him',
      '-into', '-time', '-has', '-look', '-two', '-more', '-write', '-go', '-see', '-no',
      '-way', '-could', '-people', '-my', '-than', '-first', '-water', '-been', '-call',
      '-who', '-oil', '-its', '-now', '-find');

    $options = parent::option_definition();
    $options['default_action'] = array('default' => 'default');
    $options['default_argument_type'] = array('default' => 'node');
    $options['validate']['contains']['type'] = array('default' => 'node');
    $options['boolean_mode'] = array('default' => 0);
    $options['rendered_content'] = array('default' => 0);
    $options['rendered_view_mode'] = array('default' => 'full');
    $options['adjust_relevance'] = array('default' => 1);
    $options['title_relevance'] = array('default' => 1.4);
    $options['body_relevance'] = array('default' => 0.8);
    $options['source_relevance'] = array('default' => 1);
    $options['title_operator'] = array('default' => 'increase');
    $options['enable_custom_operators'] = array('default' => 0);
    $options['custom_operators'] = array('default' => implode(' | ', $common_words));

    // Notify Views of individual field weights.
    // Note: the Views cache may need to be reset when fields are added.
    if (module_exists('field')) {
      $options['include_fields'] = array('default' => 0);
      foreach (similar_get_indices() as $field => $info) {
        $options['included_fields:'. $field] = array('default' => 1);
      }
    }
    return $options;
  }

  /**
   * Defines the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['not']);
    $form['default_argument_type']['#disabled'] = TRUE;

    $form['similar'] = array(
      '#type' => 'fieldset',
      '#title' => t('Similar (FULLTEXT) search options'),
    );

    // Add fields for increasing or decreasing relevance of destination fields.
    $relevance_options = array(
        '0' => '0%',
        '0.1' => '10%',
        '0.2' => '20%',
        '0.3' => '30%',
        '0.4' => '40%',
        '0.5' => '50%',
        '0.6' => '60%',
        '0.7' => '70%',
        '0.8' => '80%',
        '0.9' => '90%',
        '1.0' => '100%',
        '1.1' => '110%',
        '1.2' => '120%',
        '1.3' => '130%',
        '1.4' => '140%',
        '1.5' => '150%',
        '1.6' => '160%',
        '1.7' => '170%',
        '1.8' => '180%',
        '1.9' => '190%',
        '2.0' => '200%',
    );
    $entity_info = entity_get_info('node');
    foreach ($entity_info['view modes'] as $key => $values) {
      $render_view_mode_options[$key] = $values['label'];
    }
    $form['adjust_relevance'] = array(
      '#type' => 'checkbox',
      '#title' => t('Adjust relevance of title and body'),
      '#default_value' => $this->options['adjust_relevance'],
      '#description' => t('Adjust the relevance of content titles, and bodies.'),
      '#fieldset' => 'similar',
    );
    $form['title_relevance'] = array(
      '#type' => 'select',
      '#title' => t('Relevance of content\'s title.'),
      '#options' => $relevance_options,
      '#default_value' => $this->options['title_relevance'],
      '#fieldset' => 'similar',
      '#states' => array(
        // Hide the view mode settings until needed.
        'visible' => array(
          'input[name="options[adjust_relevance]"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['body_relevance'] = array(
      '#type' => 'select',
      '#title' => t('Relevance of content\'s body.'),
      '#options' => $relevance_options,
      '#default_value' => $this->options['body_relevance'],
      '#fieldset' => 'similar',
      '#states' => array(
        // Hide the view mode settings until needed.
        'visible' => array(
          'input[name="options[adjust_relevance]"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['rendered_content'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use rendered source content instead of body'),
      '#default_value' => $this->options['rendered_content'],
      '#description' => t('Score similarity by comparing to source content rendered in a view mode, rather than body value.'),
      '#fieldset' => 'similar',
    );
    $form['rendered_view_mode'] = array(
      '#type' => 'select',
      '#title' => t('View mode to render content'),
      '#options' => $render_view_mode_options,
      '#default_value' => $this->options['rendered_view_mode'],
      '#fieldset' => 'similar',
      '#states' => array(
        // Hide the view mode settings until needed.
        'visible' => array(
          'input[name="options[rendered_content]"]' => array('checked' => TRUE),
        ),
      ),
    );

    // Add fields for enabling matching in fields defined by field module.
    if (module_exists('field')) {

      // Checkbox to enable fields in query.
      $form['include_fields'] = array(
        '#type' => 'checkbox',
        '#title' => t('Include content fields in matching'),
        '#default_value' => !empty($this->options['include_fields']),
        '#description' => t('Enable FULLTEXT queries on fields defined with Field module.'),
        '#fieldset' => 'similar',
      );

      // Add a weight select list for each field defined by Field module.
      foreach (similar_get_indices() as $field => $info) {
        $form['included_fields:'. $field] = array(
          '#type' => 'select',
          '#title' => isset($info['label']) ? $info['label'] : $field,
          '#options' => $relevance_options,
          '#default_value' => isset($this->options['included_fields:'. $field]) ? $this->options['included_fields:'. $field] : 1,
          '#fieldset' => 'similar',
          '#states' => array(
            // Hide the field settings until needed.
            'visible' => array(
              'input[name="options[include_fields]"]' => array('checked' => TRUE),
            ),
          ),
        );
      }
    }

    $form['boolean_mode'] = array(
      '#type' => 'checkbox',
      '#title' => t('Execute search in boolean mode'),
      '#default_value' => !empty($this->options['boolean_mode']),
      '#fieldset' => 'similar',
      '#description' => t('Boolean mode enables the ability to increase or decrease the relevance of certain fields.<br />'.
                          'Note: Boolean mode may negatively affect the accuracy of view results.'),
    );
    // Add options for increasing or decreasing relevance of source node.
    $form['source_relevance'] = array(
      '#type' => 'checkbox',
      '#title' => t('Adjust title source relevance'),
      '#default_value' => !empty($this->options['source_relevance']),
      '#description' => t('Increase the relevance of words in the title of the content being viewed.'),
      '#fieldset' => 'similar',
      '#states' => array(
        // Hide the title source settings until needed.
        'visible' => array(
          'input[name="options[boolean_mode]"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['title_operator'] = array(
      '#type' => 'select',
      '#title' => t('Relevance of source content\'s title'),
      '#options' => array(
        'none' => t('No change'),
        'increase' => t('Increase title relevance'),
        'decrease' => t('Decrease title relevance'),
        'require_all' => t('Require all terms'),
        'require_some' => t('Require at least one term'),
        'require_complete' => t('Require complete title'),
        'exclude_all' => t('Exclude all terms'),
        'exclude_some' => t('Exclude at least one term'),
        'exclude_complete' => t('Exclude complete title'),
      ),
      '#default_value' => $this->options['title_operator'],
      '#fieldset' => 'similar',
      '#states' => array(
        // Hide the title source settings until needed.
        'visible' => array(
          'input[name="options[boolean_mode]"]' => array('checked' => TRUE),
          'input[name="options[source_relevance]"]' => array('checked' => TRUE),
        ),
      ),
    );

    // Add fields for creating custom relevance rules for specific words or phrases.
    $form['enable_custom_operators'] = array(
      '#type' => 'checkbox',
      '#title' => t('Set custom operators in words and phrases'),
      '#default_value' => !empty($this->options['enable_custom_operators']),
      '#fieldset' => 'similar',
      '#states' => array(
        // Hide the title source settings until needed.
        'visible' => array(
          'input[name="options[boolean_mode]"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['custom_operators'] = array(
      '#type' => 'textfield',
      '#title' => t('Operators'),
      '#default_value' => $this->options['custom_operators'],
      '#description' => t('Use boolean logical operators to customize the FULLTEXT query. Each expression should '.
                          'be separated by a vertical bar |.<br />'.
                          'Available operators:  +  -  >  <  (  )  ~  "<br />'.
                          'See http://dev.mysql.com/doc/refman/5.5/en/fulltext-boolean.html for more information.'),
      '#maxlength' => 1024,
      '#fieldset' => 'similar',
      '#states' => array(
        // Hide the operator settings until needed.
        'visible' => array(
          'input[name="options[boolean_mode]"]' => array('checked' => TRUE),
          'input[name="options[enable_custom_operators]"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  /**
   * Overrides argument's default actions list to ensure that node ID
   * from URL is provided.
   */
  public function default_actions($which = NULL) {
    $defaults = parent::default_actions();
    unset($defaults['ignore'], $defaults['not found'], $defaults['summary'], $defaults['empty']);

    if ($which) {
      if (!empty($defaults[$which])) {
        return $defaults[$which];
      }
    }
    else {
      return $defaults;
    }
  }

  /**
   * Validates that the argument works.
   */
  public function validate_arg($arg) {
    if (!parent::validate_arg($arg)) {
      return FALSE;
    }

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }
    $this->view->nids = $this->value;
    return TRUE;
  }

  /**
   * Override the behavior of title(). Get the title of the node.
   */
  function title_query() {
    $titles = array();

    $result = db_query("SELECT n.title FROM {node} n WHERE n.nid IN (:nids)", array(':nids' => $this->value));
    foreach ($result as $term) {
      $titles[] = check_plain($term->title);
    }
    return $titles;
  }

  /**
   * Builds the query.
   */
  public function query($group_by = FALSE) {
    $boolean = !empty($this->options['boolean_mode']);
    $text = '';
    $comparison_text = array(
      'title' => '',
      'body' => '',
    );

    // Since the view could have multiple nid arguments, load each node
    // and populate the $text variable with node titles and bodies.
    foreach ($this->value as $nid) {
      $node = node_load($nid);

      if ($this->options['rendered_content']) {
        $node_view = node_view($node, $this->options['rendered_view_mode'], $node->langcode);
        $node->body[$node->langcode][0]['value'] = render($node_view);
      }

      // Check language setting of content type
      $node_language_setting = config_get('node.type.' . $node->type, 'language');
      if (is_null($node_language_setting)) {
        $node->langcode = LANGUAGE_NONE;
      }

      if (isset($node->title)) {
        // Remove punctuation from the title.
        $title = preg_replace('/[^a-z0-9 _-]+/i', '', $node->title);
        // Alter the relevancy of words in the node title if option is selected.
        if ($boolean && !empty($this->options['source_relevance']) && isset($this->options['title_operator'])) {
          $title = $this->alter_node_title($title);
        }
        $text .= ' ' . $title;
      }

      if (isset($node->body) && isset($node->body[$node->langcode])) {
        // Strip tags and add slashes from the body before adding the title.
        $body = trim(addslashes(strip_tags($node->body[$node->langcode][0]['safe_value'])));
        $text .= ' ' . $body;
      }
    }

    // Perform replacements to add custom operators to node body and title words.
    if ($boolean && !empty($this->options['enable_custom_operators']) && !empty($this->options['custom_operators'])) {
      $replacements = array_map('trim', explode('|', $this->options['custom_operators']));
      $words = array_map('similar_handler_argument_nid::get_search_word', $replacements);
      $text = preg_replace($words, $replacements, $text);
      $body;
    }

    $comparison_text = array(
      'title' => trim($title),
      'body' => trim($body),
    );
    if (module_exists('field') && !empty($this->options['include_fields'])) {
      foreach ($options as $key => $option_value) {
        if (strstr($key, 'included_fields:')) {
          $field_name = str_replace('included_fields:', '', $key);
          $comparison_text[$field_name] = trim(addslashes(strip_tags($node->{$field_name}[$node->langcode][0]['safe_value'])));
        }
      }
    }

    $text = trim($text);
    $this->ensure_my_table();

    $query = $boolean ? $this->similar_build_query($comparison_text, 'IN BOOLEAN MODE') : $this->similar_build_query($comparison_text);

    // An empty string is passed for the table to bypass the table alias.
    $this->query->add_field('', $query, 'score');

    // Exclude the current node(s).
    if (count($this->value) > 1) {
      $this->query->add_where(0, 'node.nid', $this->value, 'NOT IN');
    }
    else {
      $this->query->add_where(0, 'node.nid', $this->value[0], '<>');
    }

    // Add a node_access tag.
    $this->query->add_tag('node_access');

    // Add a Similar Entries tag to indicate that filters and sort handlers are valid.
    $this->query->add_tag('similar_entries');
  }

  /**
   * Builds a query from argument configuration options.
   *
   * @param array $comparison_text
   *   The text of the current node to be used in the query. Contains keys:
   *   - title: the node title.
   *   - body: the node body.
   * @param string $modifier
   *   An optional FULLTEXT search modifer. ie 'IN BOOLEAN MODE'.
   *
   * @return string
   *   A FULLTEXT expression string.
   *
   * @see similar_handler_argument_nid::query()
   */
  public function similar_build_query($comparison_text, $modifier = '') {
    $query = array();
    if (!empty($modifier)) {
      $modifier = " $modifier";
    }

    // Match on title.
    $weight = 1;
    if (!empty($this->options['adjust_relevance']) && isset($this->options['title_relevance'])) {
      $weight = $this->options['title_relevance'];
    }
    $title = $comparison_text['title'];
    $query[] = "($weight * (MATCH (node.title) AGAINST ('$title'$modifier)))";

    // Match on body if it exists.
    if (db_table_exists('field_data_body')) {
      $weight = 1;
      if (!empty($this->options['adjust_relevance'])) {
        $weight = $this->options['body_relevance'];
      }
      $body_table = $this->query->add_table('field_data_body');
      $body = $comparison_text['body'];
      $query[] = "($weight * (MATCH ($body_table.body_value) AGAINST ('$body'$modifier)))";
    }

    // Add additional MATCH queries for fields if enabled.
    if (module_exists('field') && !empty($this->options['include_fields'])) {
      foreach (similar_get_indices() as $field => $info) {
        $weight = 1;
        if (!empty($this->options['adjust_relevance']) && isset($this->options['included_fields:'. $field])) {
          $weight = $this->options['included_fields:'. $field];
        }
        $alias = $this->query->ensure_table($info['table']);
        if (empty($alias) && !empty($this->query->relationships)) {
          foreach ($this->query->relationships as $relationship => $relationship_info) {
            $alias = $this->query->ensure_table($info['table'], $relationship);
            if (!empty($alias)) {
              break;
            }
          }
        }
        if (!empty($alias)) {
          $field_value = $comparison_text[$field];
          $query[] = "($weight * (MATCH ($alias.{$info['column']}) AGAINST ('$field_value'$modifier)))";
        }
      }
    }
    // Return a query that calculates an average score to prevent scores from fluctuating too much.
    // We multiply the score by different amounts depending on the modifier used.
    return "((". implode(" + ", $query) . ") / ". count($query) .")";
  }

  /**
   * Converts a user-defined word with operator into a search word for use
   * in preg_replace().
   *
   * @return string
   *   The word as prepared for use in a regular expression search.
   *
   * @see similar_handler_argument_nid::query()
   */
  public static function get_search_word($word) {
    return '/\b'. trim($word, '+-<>~()"') .'\b/i';
  }

  /**
   * Alters the node title's relevance for boolean searches.
   *
   * @param string $title
   *   The node title with punctuation removed.
   *
   * @return string
   *   The node title with new operators.
   *
   * @see similar_handler_argument_nid::query()
   */
  private function alter_node_title($title) {
    switch ($this->options['title_operator']) {
      case 'increase':
        return '>'. implode(' >', explode(' ', $title));
      case 'decrease':
        return '<'. implode(' <', explode(' ', $title));
      case 'require_all':
        return '+'. implode(' +', explode(' ', $title));
      case 'require_some':
        return "+($title)";
      case 'require_complete':
        return '"'. $title .'"';
      case 'exclude_all':
        return '-'. implode(' -', explode(' ', $title));
      case 'exclude_some':
        return "-($title)";
      case 'exclude_complete':
        return '-("'. $title .'")';
      default:
        return $title;
    }
  }

}
