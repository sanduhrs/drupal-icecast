<?php
// $Id$
/**
 * @file
 * Provide views data and handlers for yp.module.
 */

/**
 * Implementation of hook_views_data().
 */
function yp_views_data() {
  $data['yp_stream']['table']['group'] = t('Icecast YP');
  $data['yp_stream']['table']['base'] = array(
    'field' => 'sid',
    'title' => t('Icecast YP'),
    'help' => t('Icecast streams listed by the Icecast YP directory module.'),
  );
  $schema = drupal_get_schema('yp_stream');
  foreach ($schema['fields'] as $key => $field) {
    $data['yp_stream'][$key] = array(
      'title' => $field['description'],
      'help' => $field['description'] .'.',
      'field' => array(
        'click sortable' => TRUE,
      ),
    );
  }
  $data['yp_stream']['server_name']['field']['handler'] = 'views_handler_field_yp_server_name';
  $data['yp_stream']['current_song']['field']['handler'] = 'views_handler_field_yp_current_song';
  $data['yp_stream']['listen_url']['field']['handler'] = 'views_handler_field_yp_listen_url';
  $data['yp_stream']['server_type']['field']['handler'] = 'views_handler_field_yp_server_type';
  $data['yp_stream']['bitrate']['field']['handler'] = 'views_handler_field_yp_bitrate';
  $data['yp_stream']['listeners']['field']['handler'] = 'views_handler_field_numeric';
  $data['yp_stream']['max_listeners']['field']['handler'] = 'views_handler_field_numeric';
  $data['yp_stream']['last_touch']['field']['handler'] = 'views_handler_field_date';
  return $data;
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
 * Render stream type as an abbreviation.
 */
class views_handler_field_yp_server_type extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    if ($value == 'application/ogg' || $value == 'Ogg Vorbis') {
      $value = 'Ogg';
    }
    if ($value == 'audio/mpeg' || $value == 'MP3 audio' || $value == 'application/mp3') {
      $value = 'MP3';
    }
    return check_plain($value);
  }
}

/**
 * Render stream bitrate with a "k" for kbps.
 */
class views_handler_field_yp_bitrate extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    $value = $value ? $value .'k' : $value;
    return check_plain($value);
  }
}
