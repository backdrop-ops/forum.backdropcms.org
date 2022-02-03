<?php

namespace Backdrop\xautoload\Phases;

use Backdrop\xautoload\ClassFinder\ExtendedClassFinderInterface;
use Backdrop\xautoload\ClassFinder\Plugin\BackdropCoreRegistryPlugin;

class BackdropCoreRegistryRegistrator implements PhaseObserverInterface {

  /**
   * Wake up after a cache fail.
   *
   * @param ExtendedClassFinderInterface $finder
   *   The class finder object, with any cache decorator peeled off.
   * @param string[] $extensions
   *   Currently enabled extensions. Extension type by extension name.
   */
  public function wakeUp(ExtendedClassFinderInterface $finder, array $extensions) {
    $plugin = new BackdropCoreRegistryPlugin(BACKDROP_ROOT . '/');
    $finder->getNamespaceMap()->registerDeepPath('', 'registry', $plugin);
    $finder->getPrefixMap()->registerDeepPath('', 'registry', $plugin);
  }

  /**
   * Enter the boot phase of the request, where all bootstrap module files are included.
   */
  public function enterBootPhase() {
    // Nothing.
  }

  /**
   * Enter the main phase of the request, where all module files are included.
   */
  public function enterMainPhase() {
    // Nothing.
  }

  /**
   * React to new extensions that were just enabled.
   *
   * @param string $name
   * @param string $type
   */
  public function welcomeNewExtension($name, $type) {
    // Nothing.
  }

  /**
   * React to xautoload_modules_enabled()
   *
   * @param string[] $modules
   *   New module names.
   */
  public function modulesEnabled($modules) {
    // Nothing.
  }
}
