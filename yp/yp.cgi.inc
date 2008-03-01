<?php
// $Id$

function yp_map(&$stream = array(), $local, $global, $default = NULL) {
  if (isset($_POST[$global]) && $_POST[$global]) {
    $stream[$local] = trim($_POST[$global]);
  }
  elseif (isset($default)) {
    $stream[$local] = $default;
  }
}

function yp_cgi() {
  if (variable_get('yp_debug', 0)) {
    watchdog('yp', check_plain(print_r($_POST, TRUE)));
  }
  $action = isset($_POST['action']) ? trim($_POST['action']) : '';
  $stream = array();
  if ($action == 'add') {
    yp_map($stream, 'server_name', 'sn', '');
    yp_map($stream, 'server_type', 'type', '');
    yp_map($stream, 'genre', 'genre', '');
    yp_map($stream, 'bitrate', 'audio_bitrate', '');
    yp_map($stream, 'bitrate', 'b');
    yp_map($stream, 'bitrate', 'bitrate');
    yp_map($stream, 'bitrate', 'ice-bitrate');
    yp_map($stream, 'samplerate', 'audio_samplerate', '');
    yp_map($stream, 'samplerate', 'samplerate');
    yp_map($stream, 'samplerate', 'ice-samplerate');
    yp_map($stream, 'channels', 'audio_channels', '');
    yp_map($stream, 'channels', 'channels');
    yp_map($stream, 'channels', 'ice-channels');
    yp_map($stream, 'listen_url', 'listenurl', '');
    yp_map($stream, 'description', 'desc', '');
    yp_map($stream, 'url', 'url', '');
    $listing_ip = $_SERVER['REMOTE_ADDR'];
    db_query("DELETE FROM {yp_stream} WHERE listing_ip = '%s' AND listen_url = '%s'", $listing_ip, $stream['listen_url']);
    db_query("INSERT INTO {yp_stream} (server_name, server_type, genre, bitrate, listen_url, description, url, listing_ip, last_touch, channels, samplerate) 
      VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s')", $stream['server_name'], $stream['server_type'], $stream['genre'], $stream['bitrate'], 
      $stream['listen_url'], $stream['description'], $stream['url'], $listing_ip, time(), $stream['channels'], $stream['samplerate']);
    $sid = db_last_insert_id('yp_stream', 'sid');
    $yp_response = $sid ? 1 : 0;
    header('SID: '. $sid);
    header('TouchFreq: 300');
    header('YPMessage: Added');
    header('YPResponse: '. $yp_response);
    db_query("DELETE FROM {yp_stream} WHERE %d - last_touch > 900", time());
    return;
  }
  elseif ($action == 'touch') {
    yp_map($stream, 'sid', 'sid', 0);
    yp_map($stream, 'current_song', 'st', '');
    yp_map($stream, 'listeners', 'listeners', 0);
    db_query("UPDATE {yp_stream} SET listeners = %d, current_song = '%s', last_touch = %d WHERE sid = %d", $stream['listeners'], $stream['current_song'], time(), $stream['sid']);
    if (db_affected_rows()) {
      $yp_response = 1;
      $yp_message = 'Touched';
    }
    else {
      $yp_response = 0;
      $yp_message = 'SID not found';
    }
    header('YPResponse: '. $yp_response);
    header('YPMessage: '. $yp_message);
    return;
  } 
  elseif ($action == 'remove') {
    yp_map($stream, 'sid', 'sid', 0);
    db_query("DELETE FROM {yp_stream} WHERE sid = %d", $stream['sid']);
    header('YPResponse: 1');
    header('YPMessage: Removed');
    return;
  }
}
