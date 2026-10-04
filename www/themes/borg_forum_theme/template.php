<?php
/**
 * @file
 * Theme function overrides.
 */

/*******************************************************************************
 * Alter Functions
 ******************************************************************************/

function borg_forum_theme_form_comment_node_forum_topic_form_alter(&$form, $form_state) {
  // Hide the weird 'Your name' info.
  $form['author']['_author']['#access'] = FALSE;
}

/*******************************************************************************
 * Preprocess Functions
 ******************************************************************************/

/**
 * Prepares variables for node templates.
 * @see node.tpl.php
 */
function borg_forum_theme_preprocess_node(&$variables) {
  $node = $variables['node']; // Nice shorthand.
  if ($node->changed != $node->created) {
    $updated = format_date($node->changed, 'short');
    $updated_text = t('Updated: !date', array('!date' => $updated));
    $variables['submitted'] .= '&nbsp;---&nbsp;<span class="updated">' . $updated_text . '</span>';
  }
}

/**
 * Prepares variables for layout templates.
 * - Repeat code from base theme (?!?)
 * @see layout.tpl.php
 */
function borg_forum_theme_preprocess_layout(&$variables) {
  // backdrop_is_front_page() returns FALSE with missing svg icon in css
  if (backdrop_is_front_page()) {
    dpm('is front page');
    $css_path = backdrop_get_path('theme', 'borg') . '/css/page-front.css';
    dpm($css_path);
    backdrop_add_css($css_path);
  }
}

/**
 * Preprocess header templates.
 * @see header.tpl.php
 */
function borg_forum_theme_preprocess_header(&$variables) {
  $variables['branding_classes'] = array('col-xs-9', 'col-sm-6', 'col-md-5', 'col-lg-4');
  $variables['navigation_classes'] = array('col-xs-3', 'col-sm-6', 'col-md-7', 'col-lg-8');
}

/*******************************************************************************
 * Theme function Overrides
 ******************************************************************************/
