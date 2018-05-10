<?php
/**
 * @file
 * Defines the score filter handler for Similar entries module.
 */

/**
 * Defines the score filter handler.
 */
class similar_handler_filter_similarity extends views_handler_filter_numeric {

  /**
   * Defines default values for the operator and value.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['operator'] = array('default' => '>');
    $options['value']['contains']['value']['default'] = '1';
    return $options;
  }

  public function can_expose() {
    return FALSE;
  }

  /**
   * Add a similarity score selector to the value form.
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    if (empty($form_state['exposed'])) {
      $form['value']['value'] = array(
        '#type' => 'select',
        '#title' => t('Similarity score'),
        '#options' => array('0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10'),
        '#default_value' => !empty($this->value['value']) ? $this->value['value'] : 1,
      );
    }
    $form['value']['value']['#prefix'] = '<div id="edit-options-value-wrapper">';
    $form['value']['value']['#suffix'] = '</div>';
  }

  /**
   * Alters the settings form to make the value a select list instead of text.
   */
  public function options_form(&$form, &$form_state) {
    // The between/not between operators don't make sense for similarity score.
    parent::options_form($form, $form_state);
    unset($form['operator']['#options']['between'], $form['operator']['#options']['not between']);
  }

  /**
   * Inserts a having clause into the query.
   */
  public function query() {
    if (in_array('similar_entries', $this->query->tags)) {
      $this->ensure_my_table();
      $this->query->add_having('nid', 'score', $this->value['value'] * 10, $this->operator);
    }
  }

}
