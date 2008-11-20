<?php
// $Id$

/**
 * Render stream bitrate with a "k" for kbps.
 */
class views_handler_field_yp_bitrate extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    $value = is_numeric($value) ? $value . 'k' : str_replace('Quality ', '', $value);
    return check_plain($value);
  }
}

/**
 * Render "unknown" current song as empty string.
 */
class views_handler_field_yp_current_song extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    $value = ($value == 'unknown - unknown') ? '' : $value;
    return check_plain($value);
  }
}

/**
 * Render listen URL as a .M3U link.
 */
class views_handler_field_yp_listen_url extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias} .'.m3u';
    return l(theme('image', drupal_get_path('module', 'yp') .'/listen.png', t('Listen'), t('Listen')), $value, array('html' => TRUE)) . l(t('Listen'), $value);
  }
}

/**
 * Render stream URL as a link.
 */
class views_handler_field_yp_server_name extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['url'] = 'url';
  }

  function render($values) {
    return l($values->{$this->field_alias}, $values->{$this->aliases['url']});
  }
}

/**
 * Render stream type as an abbreviation.
 */
class views_handler_field_yp_server_type extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    switch ($value) {
      case 'application/ogg':
      case 'Ogg Vorbis':
        $value = 'Ogg';
        break;
      case 'audio/mpeg':
      case 'MP3 audio':
      case 'application/mp3':
        $value = 'MP3';
        break;
      case 'audio/aacp':
        $value = 'AAC+';
        break;
    }
    return check_plain($value);
  }
}