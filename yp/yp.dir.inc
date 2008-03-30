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
    array('data' => t('Name'), 'field' => 'server_name'),
    array('data' => t('Genre'), 'field' => 'genre'),
    array('data' => t('Description'), 'field' => 'description'),
    array('data' => t('Type'), 'field' => 'server_type'),
    array('data' => t('Bit rate'), 'field' => 'bitrate'),
    array('data' => t('Current track'), 'field' => 'current_song'),
    array('data' => t('Listeners'), 'field' => 'listeners', 'colspan' => 2),
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
    if ($stream['server_type'] == 'application/ogg' || $stream['server_type'] == 'Ogg Vorbis') {
      $stream['server_type'] = 'Ogg';
    }
    if ($stream['server_type'] == 'audio/mpeg' || $stream['server_type'] == 'MP3 audio' || $stream['server_type'] == 'application/mp3') {
      $stream['server_type'] = 'MP3';
    }
    $stream['current_song'] = ($stream['current_song'] == 'unknown - unknown') ? '' : $stream['current_song'];
    if ($stream['listen_url']) {
      $stream['listen_url'] .= '.m3u';
    } 
    $rows[] = array(
      yp_div(l($stream['server_name'], $stream['url'])),
      yp_div(check_plain($stream['genre'])),
      yp_div(check_plain($stream['description'])),
      check_plain($stream['server_type']),
      check_plain($stream['bitrate']),
      yp_div(check_plain($stream['current_song'])),
      array('data' => $stream['listeners'], 'align' => 'right'),
      array('data' => l(theme('image', drupal_get_path('module', 'yp') .'/listen.png', t('Listen'), t('Listen')), $stream['listen_url'], array('html' => TRUE))
      . l(t('Listen'), $stream['listen_url']), 'align' => 'right'),
    );
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => '<em>'. t('There are currently no streams available.') .'</em>', 'colspan' => 8));
  }
  return theme('table', $headers, $rows, array('class' => 'yp-dir'));
}

/**
 * Wrap a string in a <div> element.
 */
function yp_div($string) {
  return '<div>'. $string . '</div>';
}
