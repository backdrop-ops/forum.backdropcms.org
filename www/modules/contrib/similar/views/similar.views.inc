<?php
/**
 * @file
 * View hook implementations for Similar entries module.
 */

/**
 * Implements hook_views_data().
 *
 * Since fields are now a part of core in Drupal 7, we don't have
 * to create explicit relationships between node tables and field
 * tables as Views already knows about those relationships.
 *
 * @TODO: Consider supporting other types of entities?
 */
function similar_views_data() {
  // We namespace the data keys to prevent conflicts with 'similar'.
  $data['similar_entries']['table']['group'] = t('Similar Entries');
  $data['similar_entries']['table']['join'] = array('#global' => array());

  $data['similar_entries']['similarity'] = array(
    'title' => t('Similarity score'),
    'help' => t('The rating given to the related content given as an argument.'),
    'filter' => array(
      'handler' => 'similar_handler_filter_similarity',
      'help' => t('Filter by similarity score. Note: this is not accurate when the <i>include content fields</i> option is enabled in the <i>Similar entries: Nid</i> argument.'),
      'label' => 'Score',
    ),
    'field' => array(
      'handler' => 'similar_handler_field_similarity',
      'click sortable' => TRUE,
    ),
    'sort' => array('handler' => 'similar_handler_sort_similarity'),
  );
  $data['similar_entries']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('ID of content item(s). Passes content IDs to Similar entries.'),
    'argument' => array(
      'handler' => 'similar_handler_argument_nid',
      'parent' => 'views_handler_argument_numeric',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
  );

  return $data;
}
