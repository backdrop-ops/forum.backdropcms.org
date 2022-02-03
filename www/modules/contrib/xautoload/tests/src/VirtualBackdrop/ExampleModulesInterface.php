<?php


namespace Backdrop\xautoload\Tests\VirtualBackdrop;


interface ExampleModulesInterface {

  /**
   * @param string $type
   *   E.g. 'module'
   *
   * @return string[]
   *   *.module paths by module name.
   */
  public function discoverModuleFilenames($type);

  /**
   * Replicates backdrop_system_listing('/^' . BACKDROP_PHP_FUNCTION_PATTERN . '\.module$/', 'modules', 'name', 0)
   *
   * @see backdrop_system_listing()
   *
   * @return object[]
   *   E.g. array('devel' => (object)array(
   *     'uri' => 'sites/all/modules/contrib/devel/devel.module',
   *     'filename' => 'devel.module',
   *     'name' => 'devel',
   *   ));
   */
  public function backdropSystemListingModules();

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
  public function backdropParseInfoFile($name);

  /**
   * @return true[]
   */
  public function getBootstrapModules();
}
