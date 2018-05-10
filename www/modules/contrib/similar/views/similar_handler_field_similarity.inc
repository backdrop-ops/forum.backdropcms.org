<?php
/**
 * @file
 * Provides a field for displaying the similarity score of similar entries.
 */

/**
 * Similarity score field handler.
 */
class similar_handler_field_similarity extends views_handler_field {

  /**
   * Option definition.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['score_type'] = array('default' => 1);
    $options['round_decimal'] = array('default' => 0);
    $options['percent_suffix'] = array('default' => 1);
    $options['multiply_score'] = array('default' => 1);
    return $options;
  }

  /**
   * Options form for selecting between number and percentage.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['score_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display type'),
      '#default_value' => $this->options['score_type'],
      '#options' => array(
        0 => t('Show similarity score'),
        1 => t('Show as percentage'),
      ),
    );
    $form['round_decimal'] = array(
      '#type' => 'select',
      '#title' => t('Round to nearest'),
      '#default_value' => $this->options['round_decimal'],
      '#options' => array(
        0 => t('Whole number') . ' (1)',
        1 => '10th (1.0)',
        2 => '100th (1.00)',
        3 => '1,000th (1.000)',
        4 => '10,000th (1.0000)',
      ),
    );
    $form['percent_suffix'] = array(
      '#type' => 'checkbox',
      '#title' => t('Append % when showing percentage'),
      '#default_value' => !empty($this->options['percent_suffix']),
    );

    $score_options = array(
      '0.1' => '0.1', '0.2' => '0.2', '0.3' => '0.3', '0.4' => '0.4', '0.5' => '0.5',
      '0.6' => '0.6', '0.7' => '0.7', '0.8' => '0.8', '0.9' => '0.9',
    );
    for ($i = 1; $i < 101; $i++) {
      $score_options[$i] = $i;
    }
    $form['multiply_score'] = array(
      '#type' => 'select',
      '#title' => t('Multiply score by'),
      '#options' => $score_options,
      '#default_value' => $this->options['multiply_score'],
    );
  }

  function query() {}

  /**
   * Renders the score as a number or a percentage that cannot exceed 50.
   */
  function render($values) {
    $output = round($values->score * $this->options['multiply_score'], $this->options['round_decimal']);
    if ($this->options['score_type'] == 0) {
      return $output;
    }
    else {
      if (!empty($this->options['percent_suffix'])) {
        $output .= '%';
      }
      return $output;
    }
  }

}
