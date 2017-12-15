<?php
/**
 * @file
 * Default theme implementation for comments.
 */
?>
<article class="<?php print implode(' ', $classes); ?> clearfix"<?php print backdrop_attributes($attributes); ?>>
  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>

  <footer class="<?php if ($user_picture) { print 'has-picture'; } ?>">
    <?php print $permalink; ?>
    <?php print $user_picture; ?>
    <p class="submitted"><?php print $submitted; ?></p>
    <?php if ($new): ?>
      <mark class="new"><?php print $new; ?></mark>
    <?php endif; ?>
  </footer>

  <div class="content"<?php print backdrop_attributes($content_attributes); ?>>
    <?php
      // We hide the links now so that we can render them later.
      hide($content['links']);
      print render($content);
    ?>
    <?php if ($signature): ?>
    <div class="user-signature">
      <?php print $signature; ?>
    </div>
    <?php endif; ?>
  </div>

  <?php print render($content['links']) ?>
</article>
