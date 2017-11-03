<?php

/**
 * @file
 * todo.
 */

/**
 * Implements hook_views_data_alter().
 */
function borg_forum_views_data_alter(&$data) {
  // Add last reply time field
  $data['taxonomy_term_data']['last_comment'] = array(
    'title' => t('Last comment'),
    'help' => t('Last comment'),
    'field' => array(
      'handler' => 'taxonomy_term_data_handler_field_last_comment',
    ),
  );
}
