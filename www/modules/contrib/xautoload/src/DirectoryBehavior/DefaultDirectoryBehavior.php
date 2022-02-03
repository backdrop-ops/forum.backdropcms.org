<?php


namespace Backdrop\xautoload\DirectoryBehavior;

/**
 * Directory behavior for PSR-4 and PEAR.
 *
 * This class is a marker only, to be checked with instanceof.
 * @see \Backdrop\xautoload\ClassFinder\GenericPrefixMap::loadClass()
 */
final class DefaultDirectoryBehavior implements DirectoryBehaviorInterface {
}
