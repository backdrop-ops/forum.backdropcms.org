<?php


namespace Backdrop\xautoload\Tests\VirtualBackdrop;


class BackdropStatic {

  /**
   * @var array
   */
  private $data = array();

  /**
   * @var array
   */
  private $default = array();

  /**
   * Replicates backdrop_static($name, $default_value, FALSE).
   *
   * @see backdrop_static()
   *
   * @param string $name
   * @param mixed|null $default_value
   *
   * @return array
   */
  public function &get($name, $default_value = NULL) {
    if (!isset($name)) {
      throw new \InvalidArgumentException('$name cannot be NULL.');
    }
    // First check if dealing with a previously defined static variable.
    if (isset($this->data[$name]) || array_key_exists($name, $this->data)) {
      // Non-NULL $name and both $this->data[$name] and $this->default[$name] statics exist.
      return $this->data[$name];
    }
    // Neither $this->data[$name] nor $this->default[$name] static variables exist.
    // First call with new non-NULL $name. Initialize a new static variable.
    $this->default[$name] = $this->data[$name] = $default_value;
    return $this->data[$name];
  }

  /**
   * Replicates backdrop_static($name, NULL, TRUE).
   *
   * @see backdrop_static()
   *
   * @param string $name
   *
   * @return array
   */
  public function &resetKey($name) {
    if (!isset($name)) {
      throw new \InvalidArgumentException('$name cannot be NULL.');
    }
    // First check if dealing with a previously defined static variable.
    if (isset($this->data[$name]) || array_key_exists($name, $this->data)) {
      // Non-NULL $name and both $this->data[$name] and $this->default[$name] statics exist.
      // Reset pre-existing static variable to its default value.
      $this->data[$name] = $this->default[$name];
      return $this->data[$name];
    }
    // Neither $this->data[$name] nor $this->default[$name] static variables exist.
    // Reset was called before a default is set and yet a variable must be
    // returned.
    return $this->data;
  }

  /**
   * Replicates backdrop_static(NULL, NULL, TRUE).
   *
   * @see backdrop_static()
   *
   * @return array
   */
  public function &resetAll() {
    // Reset all: ($name == NULL). This needs to be done one at a time so that
    // references returned by earlier invocations of backdrop_static() also get
    // reset.
    foreach ($this->default as $name => $value) {
      $this->data[$name] = $value;
    }
    // As the function returns a reference, the return should always be a
    // variable.
    return $this->data;
  }
} 
