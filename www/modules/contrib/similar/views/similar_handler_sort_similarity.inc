<?php
/**
 * @file
 * Defines the Similar entries sort handler.
 */

/**
 * Defines the similar entries sort handler.
 */
class similar_handler_sort_similarity extends views_handler_sort {

  /**
   * Defines default options.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['order'] = array('default' => 'DESC');
    return $options;
  }

  /**
   * Defines query elements.
   */
  public function query() {
    if (in_array('similar_entries', $this->query->tags)) {
      $this->query->add_orderby(NULL, NULL, $this->options['order'], 'score');
      $this->query->add_groupby('score');
    }
  }

}
