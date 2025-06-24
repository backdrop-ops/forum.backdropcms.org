<?php

/**
 * Administrative settings for Turnstile.
 */
function turnstile_admin_settings() {
  $config = config('turnstile.settings');

  // Add some assistance around safe secret key handling.
  $config_secret_key = $config->get('secret_key');
  $secret_description = t('The secret key given to you when you <a href="@url">register for turnstile</a>.', array('@url' => 'https://www.cloudflare.com/application-services/products/turnstile'));
  if ($config_secret_key !== '') {
    $message = t('The turnstile <strong>secret key</strong> is saved in configuration. For security, please remove it from this form, and add it to <code>$settings[' . "'turnstile_secret_key'" . ']</code> in <code>settings.local.php</code>.');
    backdrop_set_message($message, 'warning');
    $secret_description .= ' ' . t('For security, place this key in<code>settings.local.php</code> instead of this location.', array('@url' => 'https://www.cloudflare.com/application-services/products/turnstile'));
  }
  else {
    $turnstile_secret_key = settings_get('turnstile_secret_key', FALSE);
    if (!$turnstile_secret_key) {
      $message = t('No turnstile <strong>secret key</strong> was found in either <code>$settings</code> or configuration.');
      backdrop_set_message($message, 'error');
      $secret_description .= ' ' . t('For security, place this key in<code>settings.local.php</code> instead of using this form.', array('@url' => 'https://www.cloudflare.com/application-services/products/turnstile'));
    }
    else {
      $message = t('A secret key was successfully located in <code>$settings</code>.');
      backdrop_set_message($message);
    }
  }

  $form['#config'] = 'turnstile.settings';
  $form['general'] = [
    '#type' => 'details',
    '#summary' => t('General settings'),
    '#open' => TRUE,
  ];

  $form['general']['site_key'] = [
    '#default_value' => $config->get('site_key'),
    '#description' => t('The site key given to you when you <a href="@url">register for Turnstile</a>.',  array('@url' => 'https://www.cloudflare.com/application-services/products/turnstile')),
    '#maxlength' => 50,
    '#required' => TRUE,
    '#title' => t('Site key'),
    '#type' => 'textfield',
  ];

  $form['general']['secret_key'] = [
    '#default_value' => $config_secret_key,
    '#description' => $secret_description,
    '#maxlength' => 50,
    '#title' => t('Secret key'),
    '#type' => 'textfield',
  ];

  $form['general']['src'] = [
    '#default_value' => $config->get('src'),
    '#description' => t('Default URL is "@url".', array('@url' => 'https://challenges.cloudflare.com/turnstile/v0/api.js')),
    '#maxlength' => 200,
    '#required' => TRUE,
    '#title' => t('Turnstile JavaScript resource URL'),
    '#type' => 'textfield',
  ];

  // Widget configurations.
  $form['widget'] = [
    '#type' => 'details',
    '#summary' => t('Widget settings'),
    '#open' => TRUE,
  ];
  $form['widget']['theme'] = [
    '#default_value' => $config->get('theme'),
    '#description' => t('Defines which theme to use for Turnstile.'),
    '#options' => [
      'light' => t('Light (default)'),
      'dark' => t('Dark'),
      'auto' => t('Auto'),
    ],
    '#title' => t('Theme'),
    '#type' => 'select',
  ];
  $form['widget']['size'] = array(
    '#default_value' => $config->get('size'),
    '#description' => t('The size to serve.'),
    '#options' => array(
      'normal' => t('Normal (default)'),
      'compact' => t('Compact'),
    ),
    '#title' => t('Size'),
    '#type' => 'select',
  );
  $form['widget']['tabindex'] = [
    '#default_value' => $config->get('tabindex'),
    '#description' => t('Set the <a href="@tabindex">tabindex</a> of the widget and challenge (Default = 0). If other elements on your page use tabindex, it should be set to make user navigation easier.', array('@tabindex' => 'https://www.w3.org/TR/html4/interact/forms.html#adef-tabindex')),
    '#maxlength' => 4,
    '#title' => t('Tabindex'),
    '#type' => 'number',
    '#min' => -1,
  ];

  return system_settings_form($form);
}
