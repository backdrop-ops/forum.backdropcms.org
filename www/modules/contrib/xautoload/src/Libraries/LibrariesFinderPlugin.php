<?php


namespace Backdrop\xautoload\Libraries;


use Backdrop\xautoload\ClassFinder\ExtendedClassFinderInterface;
use Backdrop\xautoload\ClassFinder\InjectedApi\InjectedApiInterface;
use Backdrop\xautoload\ClassFinder\Plugin\FinderPluginInterface;
use Backdrop\xautoload\BackdropSystem\BackdropSystemInterface;
use Backdrop\xautoload\Phases\PhaseObserverInterface;


/**
 * Registers autoload mappings from all libraries on hook_init(), or after the
 * first cache miss.
 */
class LibrariesFinderPlugin implements FinderPluginInterface {

  /**
   * @var ExtendedClassFinderInterface
   */
  private $finder;

  /**
   * @var BackdropSystemInterface
   */
  private $system;

  /**
   * @param ExtendedClassFinderInterface $finder
   * @param BackdropSystemInterface $system
   */
  function __construct(ExtendedClassFinderInterface $finder, BackdropSystemInterface $system) {
    $this->finder = $finder;
    $this->system = $system;
  }

  /**
   * Find the file for a class that in PSR-0 or PEAR would be in
   * $psr_0_root . '/' . $path_fragment . $path_suffix
   *
   * @param InjectedApiInterface $api
   * @param string $logical_base_path
   * @param string $relative_path
   *
   * @return bool|null
   *   TRUE, if the file was found.
   *   FALSE or NULL, otherwise.
   */
  function findFile($api, $logical_base_path, $relative_path) {

    // Prevent recursion if this is called from libraries_info().
    // @todo Find a better way to do this?
    $backtrace = defined('DEBUG_BACKTRACE_IGNORE_ARGS')
      ? debug_backtrace(DEBUG_BACKTRACE_IGNORE_ARGS)
      : debug_backtrace(FALSE);
    foreach ($backtrace as $call) {
      if ('libraries_info' === $call['function']) {
        return FALSE;
      }
    }

    $this->finder->getNamespaceMap()->unregisterDeepPath('', '');
    $this->finder->getPrefixMap()->unregisterDeepPath('', '');
    $this->registerAllLibraries();
    return $this->finder->apiFindFile($api, $api->getClass());
  }

  /**
   * Registers all libraries that have an "xautoload" setting.
   */
  private function registerAllLibraries() {
    $adapter = \xautoload_InjectedAPI_hookXautoload::create($this->finder, '');
    foreach ($info = $this->getLibrariesXautoloadInfo() as $name => $pathAndCallback) {
      list($path, $callback) = $pathAndCallback;
      if (!is_callable($callback)) {
        continue;
      }
      if (!is_dir($path)) {
        continue;
      }
      $adapter->setExtensionDir($path);
      call_user_func($callback, $adapter, $path);
    }
  }

  /**
   * @return array[]
   */
  private function getLibrariesXautoloadInfo() {
    $cached = $this->system->cacheGet(XAUTOLOAD_CACHENAME_LIBRARIES_INFO);
    if (FALSE !== $cached) {
      return $cached->data;
    }
    $info = $this->buildLibrariesXautoloadInfo();
    $this->system->cacheSet(XAUTOLOAD_CACHENAME_LIBRARIES_INFO, $info);
    return $info;
  }

  /**
   * @return array[]
   */
  private function buildLibrariesXautoloadInfo() {
    // @todo Reset backdrop_static('libraries') ?
    $all = array();
    foreach ($this->system->getLibrariesInfo() as $name => $info) {
      if (!isset($info['xautoload'])) {
        continue;
      }
      $callback = $info['xautoload'];
      if (!is_callable($callback)) {
        continue;
      }
      /** See https://www.drupal.org/node/2473901 */
      $path = isset($info['library path'])
        ? $info['library path']
        : $this->system->librariesGetPath($name);
      if (FALSE === $path) {
        continue;
      }
      $all[$name] = array($path, $callback);
    }
    return $all;
  }

}
