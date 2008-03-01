<?php
// $Id$

/**
 * Callback function for YP directory.
 */
function yp_dir() {
  return theme('yp_dir');
}

/**
 * Render a YP directory.
 */
function theme_yp_dir() {
  $headers = array(
    array('data' => t('Name'), 'field' => 'server_name', 'sort' => 'desc'), 
    array('data' => t('Type'), 'field' => 'server_type'), 
    array('data' => t('Genre'), 'field' => 'genre'), 
    array('data' => t('Description'), 'field' => 'description'), 
    array('data' => t('Bit rate'), 'field' => 'bitrate'),
  //  array('data' => t('Sample rate'), 'field' => 'samplerate'), 
  //  array('data' => t('Channels'), 'field' => 'channels'),
    array('data' => t('Listeners'), 'field' => 'listeners'), 
    array('data' => t('Current track'), 'field' => 'current_song'), 
    '',
  );
  $streams = db_query("SELECT * FROM {yp_stream} WHERE %d - last_touch < 300 ". tablesort_sql($headers), time());
  while ($stream = db_fetch_array($streams)) {
    if (strstr($stream['bitrate'], 'Quality')) {
      $stream['bitrate'] = '';
    }
    if ($stream['bitrate']) {
      $stream['bitrate'] .= t('k');
    }
    if (!$stream['bitrate']) {
      $stream['bitrate'] = '';
    }
    if ($stream['channels'] == 2) {
      $stream['channels'] = t('stereo');
    }
    if ($stream['channels'] == 1) {
      $stream['channels'] = t('mono');
    }
    if ($stream['samplerate']) {
      $stream['samplerate'] = sprintf("%.0f",$stream['samplerate']/1000) . t('khz');
    }
    if ($stream['server_type'] == 'application/ogg' || $stream['server_type'] == 'Ogg Vorbis') {
      $stream['server_type'] = 'Ogg';
    }
    if ($stream['server_type'] == 'audio/mpeg' || $stream['server_type'] == 'MP3 audio') {
      $stream['server_type'] = 'MP3';
    }
    $stream['current_song'] = ($stream['current_song'] == 'unknown - unknown') ? '' : $stream['current_song'];
    if ($stream['listen_url']) {
      $stream['listen_url'] .= '.m3u';
    } 
    $rows[] = array(
      l($stream['server_name'], $stream['url']),
      check_plain($stream['server_type']),
      check_plain($stream['genre']),
      check_plain($stream['description']),
      check_plain($stream['bitrate']),
    //  l($stream['samplerate'], $stream['listen_url']),
    //  l($stream['channels'], $stream['listen_url']),
      $stream['listeners'], // Integer field
      check_plain($stream['current_song']),
      l(theme('image', drupal_get_path('module', 'yp') .'/listen.png', t('Listen'), t('Listen')), $stream['listen_url'], array('html' => TRUE))
      . l(t('Listen'), $stream['listen_url']),
    );
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => '<em>'. t('There are currently no streams available.') .'</em>', 'colspan' => 8));
  }
  return theme('table', $headers, $rows);
}
