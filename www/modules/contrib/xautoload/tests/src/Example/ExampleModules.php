<?php


namespace Backdrop\xautoload\Tests\Example;


use Backdrop\xautoload\Tests\BackdropBootTest\AbstractBackdropBootTest;
use Backdrop\xautoload\Tests\Filesystem\VirtualFilesystem;

class ExampleModules extends AbstractExampleModules {

  /**
   * @return string[]
   */
  public function getAvailableExtensions() {
    return array_fill_keys(array(
        'system', 'xautoload', 'libraries',
        'testmod_pearflat', 'testmod_psr0_lib', 'testmod_psr4_custom', 'testmod_psr4_src',
      ), 'module');
  }

  /**
   * @return string[]
   */
  public function getExampleClasses() {
    return array(
      'testmod_pearflat' => 'testmod_pearflat_Foo',
      'testmod_psr0_lib' => 'Backdrop\testmod_psr0_lib\Foo',
      'testmod_psr4_custom' => 'Backdrop\testmod_psr4_custom\Foo',
      'testmod_psr4_src' => 'Backdrop\testmod_psr4_src\Foo',
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
    if (0 === strpos($name, 'testmod')) {
      $info['dependencies'][] = 'xautoload';
    }
    return $info;
  }

}
