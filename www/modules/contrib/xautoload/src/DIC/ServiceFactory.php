<?php

namespace Backdrop\xautoload\DIC;

use Backdrop\xautoload\Adapter\ClassFinderAdapter;
use Backdrop\xautoload\Adapter\BackdropExtensionAdapter;
use Backdrop\xautoload\ClassFinder\ClassFinder;
use Backdrop\xautoload\ClassFinder\ClassFinderInterface;
use Backdrop\xautoload\CacheManager\CacheManager;
use Backdrop\xautoload\ClassFinder\ProxyClassFinder;
use Backdrop\xautoload\Discovery\CachedClassMapGenerator;
use Backdrop\xautoload\Discovery\ClassMapGenerator;
use Backdrop\xautoload\BackdropSystem\BackdropSystem;
use Backdrop\xautoload\BackdropSystem\BackdropSystemInterface;
use Backdrop\xautoload\Libraries\LibrariesInfoAlter;
use Backdrop\xautoload\Phases\BackdropCoreRegistryRegistrator;
use Backdrop\xautoload\Phases\BackdropPhaseControl;
use Backdrop\xautoload\Phases\ExtensionNamespaces;
use Backdrop\xautoload\Phases\HookXautoload;
use Backdrop\xautoload\Libraries\LibrariesOnInit;
use Backdrop\xautoload\Main;

/**
 * @see ServiceContainerInterface
 * @see ServiceContainer
 */
class ServiceFactory {

  /**
   * @param ServiceContainer $services
   *
   * @return Main
   */
  function main($services) {
    return new Main($services);
  }

  /**
   * @param ServiceContainer $services
   *
   * @return ClassFinderAdapter
   */
  function adapter($services) {
    return new ClassFinderAdapter($services->finder, $services->classMapGenerator);
  }

  /**
   * @param ServiceContainer $services
   *
   * @return ClassMapGenerator
   */
  function classMapGenerator($services) {
    return new CachedClassMapGenerator($services->classMapGeneratorRaw, $services->system);
  }

  /**
   * @param ServiceContainer $services
   *
   * @return ClassMapGenerator
   */
  function classMapGeneratorRaw($services) {
    return new ClassMapGenerator();
  }

  /**
   * @param ServiceContainer $services
   *
   * @return BackdropExtensionAdapter
   */
  function extensionRegistrationService($services) {
    return new BackdropExtensionAdapter($services->system, $services->finder);
  }

  /**
   * @param ServiceContainer $services
   *
   * @return CacheManager
   */
  function cacheManager($services) {
    return CacheManager::create($services->system);
  }

  /**
   * Proxy class finder.
   *
   * @param ServiceContainer $services
   *
   * @return ClassFinderInterface
   *   Proxy object wrapping the class finder.
   *   This is used to delay namespace registration until the first time the
   *   finder is actually used.
   *   (which might never happen thanks to the APC cache)
   */
  function proxyFinder($services) {
    // The class finder is cheap to create, so it can use an identity proxy.
    return new ProxyClassFinder($services->finder);
  }

  /**
   * The class finder (alias for 'finder').
   *
   * @param ServiceContainer $services
   *
   * @return ClassFinderInterface
   *   Object that can find classes,
   *   and provides methods to register namespaces and prefixes.
   *   Note: The findClass() method expects an InjectedAPI argument.
   */
  function classFinder($services) {
    return $services->finder;
  }

  /**
   * The class finder (alias for 'classFinder').
   *
   * @param ServiceContainer $services
   *
   * @return ClassFinderInterface
   *   Object that can find classes,
   *   and provides methods to register namespaces and prefixes.
   *   Notes:
   *   - The findClass() method expects an InjectedAPI argument.
   *   - namespaces are only supported since PHP 5.3
   */
  function finder($services) {
    return new ClassFinder();
  }

  /**
   * @param ServiceContainer $services
   *
   * @return BackdropSystemInterface
   */
  function system($services) {
    return new BackdropSystem();
  }

  /**
   * @param ServiceContainer $services
   *
   * @return BackdropPhaseControl
   */
  function phaseControl($services) {
    $observers = array(
      $services->extensionNamespaces,
      new HookXautoload($services->system),
      new LibrariesOnInit($services->system),
    );
    return new BackdropPhaseControl($services->system, $observers);
  }

  /**
   * @param ServiceContainer $services
   *
   * @return ExtensionNamespaces
   */
  function extensionNamespaces($services) {
    return new ExtensionNamespaces($services->system);
  }

  /**
   * @param ServiceContainer $services
   *
   * @return LibrariesInfoAlter
   */
  function librariesInfoAlter($services) {
    return new LibrariesInfoAlter();
  }

}

