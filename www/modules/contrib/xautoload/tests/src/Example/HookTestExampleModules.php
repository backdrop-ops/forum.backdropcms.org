<?php


namespace Backdrop\xautoload\Tests\Example;


use Backdrop\xautoload\Tests\BackdropBootTest\AbstractBackdropBootTest;
use Backdrop\xautoload\Tests\Filesystem\VirtualFilesystem;

/**
 * @see BackdropBootHookTest
 */
class HookTestExampleModules extends AbstractExampleModules {

  /**
   * @return array[]
   */
  public function getAvailableExtensions() {
    return array_fill_keys(array(
        'system', 'xautoload', 'libraries',
        'testmod',
      ), 'module');
  }

  /**
   * @return string[]
   */
  public function getExampleClasses() {
    return array(
      'testmod' => array(
        'Backdrop\\testmod\\Foo',
        'Acme\\TestLib\\Foo',
      ),
    );
  }

  /**
   * Replicates backdrop_parse_info_file(dirname($module->uri) . '/' . $module->name . '.info')
   *
   * @see backdrop_parse_info_file()
   *
   * @param string $name
   *
   * @return array
   *   Parsed info file contents.
   */
  public function backdropParseInfoFile($name) {
    $info = array('core' => '7.x');
    if ('testmod' === $name) {
      $info['dependencies'][] = 'xautoload';
      $info['dependencies'][] = 'libraries';
    }
    return $info;
  }

}
