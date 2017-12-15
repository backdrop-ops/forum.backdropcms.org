<?php
/**
 * @file
 * Theme function overrides.
 */

/*******************************************************************************
 * Preprocess Functions
 ******************************************************************************/

/**
 * Prepares variables for node template.
 * @see node.tpl.php
 */
function borg_forum_theme_preprocess_node(&$variables) {
  $key = array_search('inline', $variables['content']['links']['#attributes']['class']);
  unset($variables['content']['links']['#attributes']['class'][$key]);
}

/**
 * Prepares variables for comment template.
 * @see comment.tpl.php
 */
function borg_forum_theme_preprocess_comment(&$variables) {
  $variables['submitted'] = str_replace('Submitted by', 'Comment from', $variables['submitted']);
}

/*******************************************************************************
 * Theme function Overrides
 ******************************************************************************/
