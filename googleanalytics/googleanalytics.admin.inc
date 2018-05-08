<?php

function googleanalytics_admin_settings($form, &$form_state) {
  $config = config('googleanalytics.settings');

  $form['trackingId'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Analytics Tracking ID'),
    '#default_value' => $config->get('trackingId'),
    '#description' => t('Enter Your Google Analytics Tracking ID. Format should be UA-12345678-9'),
  );

  return system_config_form($form, $form_state);
}

function googleanalytics_admin_settings_submit($form, $form_state) {
  config('googleanalytics.settings')
    ->set('trackingId', $form_state['values']['trackingId'])
    ->save();
}
