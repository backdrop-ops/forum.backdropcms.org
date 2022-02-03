<?php
namespace Backdrop\xautoload\DIC;

use Backdrop\xautoload\Adapter\ClassFinderAdapter;
use Backdrop\xautoload\Adapter\BackdropExtensionAdapter;
use Backdrop\xautoload\ClassFinder\ExtendedClassFinderInterface;
use Backdrop\xautoload\CacheManager\CacheManager;
use Backdrop\xautoload\ClassFinder\ProxyClassFinder;
use Backdrop\xautoload\Discovery\ClassMapGenerator;
use Backdrop\xautoload\Discovery\ClassMapGeneratorInterface;
use Backdrop\xautoload\BackdropSystem\BackdropSystemInterface;
use Backdrop\xautoload\Libraries\LibrariesInfoAlter;
use Backdrop\xautoload\Phases\BackdropPhaseControl;
use Backdrop\xautoload\Phases\ExtensionNamespaces;
use Backdrop\xautoload\Main;

/**
 * @property Main $main
 * @property ClassFinderAdapter $adapter
 * @property ClassMapGeneratorInterface $classMapGenerator
 * @property ClassMapGenerator $classMapGeneratorRaw
 * @property CacheManager $cacheManager
 * @property ProxyClassFinder $proxyFinder
 * @property ExtendedClassFinderInterface $classFinder
 * @property ExtendedClassFinderInterface $finder
 *   Alias for ->classFinder
 * @property BackdropSystemInterface $system
 * @property BackdropPhaseControl $phaseControl
 * @property BackdropExtensionAdapter $extensionRegistrationService
 * @property ExtensionNamespaces extensionNamespaces
 * @property LibrariesInfoAlter librariesInfoAlter
 *
 * @see \Backdrop\xautoload\DIC\ServiceContainer
 * @see \Backdrop\xautoload\DIC\ServiceFactory
 */
interface ServiceContainerInterface {

  /**
   * Retrieves a lazy-instantiated service.
   *
   * @param string $key
   *   A key to specify a service.
   * @return mixed
   *   The service for the given key. Usually an object.
   */
  function __get($key);
}
