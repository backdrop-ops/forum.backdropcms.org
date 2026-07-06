<?php

/**
 * Administrative settings for Turnstile.
 */
function turnstile_admin_settings() {
  $config = config('turnstile.settings');

  $form['#config'] = 'turnstile.settings';

  $help = '';
  $help .= '<h3>' . t('About') . '</h3>';
  $help .= t("<p>Cloudflare Turnstile delivers frustration-free, CAPTCHA-free web experiences to website visitors - with just a simple snippet of free code.</p><p>What's more, Turnstile stops abuse and confirms visitors are real without the data privacy concerns or awful UX that CAPTCHAs thrust on users.</p>");

  $help .= '<p>';
  $help .= t('Visit the <a href="@project_link">Turnstile project page</a> on backdropcms.org for more information.', array(
    '@project_link' => 'https://www.backdropcms.org/project/turnstile',
  ));
  $help .= '</p>';

  $form['help'] = array(
    '#type' => 'help',
    '#markup' => $help,
  );

  $form['general'] = [
    '#type' => 'details',
    '#summary' => t('General settings'),
    '#open' => TRUE,
  ];

  $form['general']['site_key'] = [
    '#default_value' => $config->get('site_key'),
    '#description' => t('The site key given to you when you <a href="@url">register for Turnstile</a>.', array('@url' => 'https://cloudflare.com')),
    '#maxlength' => 50,
    '#required' => TRUE,
    '#title' => t('Site key'),
    '#type' => 'textfield',
  ];

  $form['general']['secret_key'] = [
    '#default_value' => $config->get('secret_key'),
    '#description' => t('The secret key given to you when you <a href="@url">register for Turnstile</a>. For security, place this key in <code>settings.local.php</code> instead saving with this form. Use the format: <code>$config[\'turnstile.settings\'][\'secret_key\'] = "12345";</code>', array('@url' => 'https://cloudflare.com')),
    '#maxlength' => 50,
    '#required' => TRUE,
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
