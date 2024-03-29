<?php
/**
 * @file
 * Defines date-related Views data and plugins:
 *
 * Date argument:
 *   A generic date argument that has an option to select one or more
 *   Views date fields to filter on, automatically adds them to the view,
 *   and then filters the view by the value of the selected field(s).
 *   The flexible argument will accept and evaluate most ISO date
 *   and period formats, like 2009-05-01, 2008-W25, P1W.
 *
 * Date filter:
 *   A generic date filter that has an option to select a
 *   Views date field to filter on, with a choice of a widget to use
 *   for the filter form and an option to set the default value to
 *   a set date or something like 'now +90 days' . If the operator is
 *   set to 'between' or 'not between' you can set a default value for
 *   both the start and end dates.
 *
 * Current date argument default
 *   Adds a default option to set the argument to the current date
 *   when the argument is empty.
 *
 * Date navigation attachment
 *   Navigation that can be attached to any display to create back/next
 *   links by date, requires the date argument and uses the current
 *   date argument default to set a starting point for the view.
 */

/**
 * Implements hook_views_plugins().
 */
function date_views_plugins() {
  $path = backdrop_get_path('module', 'date');
  module_load_include('inc', 'date', 'date.theme');

  return array(
    'module' => 'date',
    'pager' => array(
      'date_views_pager' => array(
        'title' => t('Page by date'),
        'help' => t('Page using the value of a date field.'),
        'handler' => 'date_views_plugin_pager',
        'path' => "$path/includes",
        'help topic' => 'date-views-pager',
        'uses options' => TRUE,
      ),
    ),
  );
}

/**
 * Implements hook_views_data().
 */
function date_views_data() {
  $data = array();

  $tables = date_views_base_tables();

  foreach ($tables as $base_table => $entity) {
    // The flexible date argument.
    $data[$base_table]['date_argument'] = array(
      'group' => t('Date'),
      'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
      'help' => t('Filter any Views !base_table date field by a date argument, using any common ISO date/period format (i.e. YYYY, YYYY-MM, YYYY-MM-DD, YYYY-W99, YYYY-MM-DD--P3M, P90D, etc).', array('!base_table' => $base_table)),
      'argument' => array(
        'handler' => 'date_views_argument_handler',
        'empty field name' => t('Undated'),
        'is date' => TRUE,
        // 'skip base' => $base_table,
      ),
    );
    // The flexible date filter.
    $data[$base_table]['date_filter'] = array(
      'group' => t('Date'),
      'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
      'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table)),
      'filter' => array(
        'handler' => 'date_views_filter_handler',
        'empty field name' => t('Undated'),
        'is date' => TRUE,
        // 'skip base' => $base_table,
      ),
    );
  }

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function date_views_data_alter(&$data) {

  // Mark all the core date handlers as date fields.
  // This will identify all handlers that directly use the _date handlers,
  // will not pick up any that extend those handlers.
  foreach ($data as $base_table => &$table) {
    foreach ($table as $id => &$field) {
      foreach (array('field', 'sort', 'filter', 'argument') as $type) {
        if (isset($field[$type]) && isset($field[$type]['handler']) && ($field[$type]['handler'] == 'views_handler_' . $type . '_date')) {
          $field[$type]['is date'] = TRUE;
        }
      }
    }
  }
}

/**
 * Central function for setting up the right timezone values.
 *
 * In the SQL date handler.
 *
 * The date handler will use this information to decide if the
 * database value needs a timezone conversion.
 *
 * In Views, we will always be comparing to a local date value,
 * so the goal is to convert the database value to the right
 * value to compare to the local value.
 */
function date_views_set_timezone(&$date_handler, &$view, $field) {
  switch ($field['tz_handling']) {
    case 'date':
      $date_handler->db_timezone = 'UTC';
      $date_handler->local_timezone_field = $field['timezone_field'];
      $date_handler->offset_field = $field['offset_field'];
      break;

    case 'none':
      $date_handler->db_timezone = date_default_timezone();
      $date_handler->local_timezone = date_default_timezone();
      break;

    case 'utc':
      $date_handler->db_timezone = 'UTC';
      $date_handler->local_timezone = 'UTC';
      break;

    default:
      $date_handler->db_timezone = 'UTC';
      $date_handler->local_timezone = date_default_timezone();
      break;
  }
}

/**
 * Helper function to generate a query string.
 *
 * @param object $view
 *   A View object.
 *
 * @param array $extra_params
 *   An extra parameters.
 *
 * @return null/string
 *   Return a query or NULL.
 */
function date_views_querystring($view, $extra_params = array()) {
  $query_params = array_merge($_GET, $extra_params);
  // Allow NULL params to be removed from the query string.
  foreach ($extra_params as $key => $value) {
    if (!isset($value)) {
      unset($query_params[$key]);
    }
  }
  // Filter the special "q" and "view" variables out of the query string.
  $exclude = array('q');
  // If we don't explicitly add a value for "view", filter it out.
  if (empty($extra_params['view'])) {
    $exclude[] = 'view';
  }

  $query = backdrop_get_query_parameters($query_params, $exclude);
  // To prevent an empty query string from adding a "?" on to the end of a URL,
  // we return NULL.
  return !empty($query) ? $query : NULL;
}

/**
 * Helper function to map entity types to the Views base table they use, to make
 * it easier to infer the entity type from a base table.
 *
 * Views has a new handler called views_handler_field_entity() that loads
 * entities.
 *
 * And you can use something like the following to get the entity type from a
 * view, but not all our base tables contain the entity information we need,
 * (i.e. revisions).
 *
 * So it won't work here and we resort to creating information from
 * entity_get_info().
 *
 *   // A method to get the entity type for a base table.
 *   $table_data = views_fetch_data($base_table);
 *   if (!isset($table_data['table']['base']['entity type'])) {
 *     return FALSE;
 *   }
 *   $entity_type = $table_data['table']['base']['entity type'];
 */
function date_views_base_tables() {
  $base_tables = &backdrop_static(__FILE__, array());

  if (empty($base_tables)) {

    // First we get the base tables we can learn about from entity_info.
    $entity_info = entity_get_info();
    foreach ($entity_info as $entity_type => $info) {
      if (!empty($info['base table'])) {
        $base_tables[$info['base table']] = $entity_type;
      }
      if (!empty($info['revision table'])) {
        $base_tables[$info['revision table']] = $entity_type;
      }
    }

    // Then we let other modules tell us about other entity tables
    // that hold date fields.
    $base_tables += module_invoke_all('date_views_extra_tables');
  }

  return $base_tables;
}

/**
 * Custom helper function.
 *
 * The instanceof function makes this work for any handler that was derived
 * from 'views_handler_filter_date' or 'views_handler_argument_date',
 * which includes core date fields like the node updated field.
 *
 * The test for $handler->min_date tells us that this is an argument that
 * not only is derived from the views date handler but also has been processed
 * by the Date Views filter or argument code.
 */
function date_views_handler_is_date($handler, $type = 'argument') {
  switch ($type) {
    case 'filter':
      return $handler instanceof views_handler_filter_date && !empty($handler->min_date);

    case 'argument':
      return $handler instanceof views_handler_argument_date && !empty($handler->min_date);
  }
  return FALSE;
}

/**
 * Format links correctly for the new Date pager.
 */
function date_pager_url($view, $date_type = NULL, $date_arg = NULL, $force_view_url = FALSE, $absolute = TRUE) {

  // If someone adds a pager without a matching argument,
  // there is no date information to work with.
  if (empty($view->date_info) || !isset($view->date_info->date_arg_pos)) {
    return '';
  }

  $args = $view->args;
  $pos = $view->date_info->date_arg_pos;

  // The View arguments array is indexed numerically but is not necessarily
  // in numerical order. Sort the arguments to ensure the correct order.
  ksort($args);

  // If there are empty arguments before the date argument,
  // pad them with the wildcard so the date argument will be in
  // the right position.
  if (count($args) < $pos) {
    foreach ($view->argument as $name => $argument) {
      if ($argument->position == $pos) {
        break;
      }
      $args[] = $argument->options['exception']['value'];
    }
  }

  if (!empty($date_type)) {
    switch ($date_type) {
      case 'year':
        $args[$pos] = date_pad($view->date_info->year, 4);
        break;

      case 'week':
        $args[$pos] = date_pad($view->date_info->year, 4) . '-W' . date_pad($view->date_info->week);
        break;

      case 'day':
        $args[$pos] = date_pad($view->date_info->year, 4) . '-' . date_pad($view->date_info->month) . '-' . date_pad($view->date_info->day);
        break;

      default:
        $args[$pos] = date_pad($view->date_info->year, 4) . '-' . date_pad($view->date_info->month);
        break;
    }
  }
  elseif (!empty($date_arg)) {
    $args[$pos] = $date_arg;
  }
  else {
    $args = $view->args;
  }
  // Is this an embedded or a block view?
  // Return the pager query value.
  if (!$force_view_url &&
  (!empty($view->preview) || !empty($view->date_info->block_identifier))) {

    $url = $args[$pos];
    $key = date_block_identifier($view);
    if (!empty($key)) {
      return url($_GET['q'], array(
        'query' => date_views_querystring($view, array($key => $url)),
        'absolute' => $absolute));
    }
  }

  // Normal views may need query strings appended to them if they use exposed
  // filters.
  return url($view->get_url($args), array(
    'query' => date_views_querystring($view),
    'absolute' => $absolute,
    )
  );
}

/**
 * Identifier of a date block.
 */
function date_block_identifier($view) {
  if (!empty($view->block_identifier)) {
    return $view->block_identifier;
  }
  return isset($view->date_info->block_identifier) ? $view->date_info->block_identifier : NULL;
}
