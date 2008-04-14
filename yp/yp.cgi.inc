<?php
// $Id$

function yp_cgi() {
  if (variable_get('yp_debug', 0)) {
    watchdog('yp', check_plain(print_r($_POST, TRUE)), NULL, WATCHDOG_DEBUG);
  }
  $action = isset($_POST['action']) ? trim($_POST['action']) : '';
  if ($action == 'add') {
    new yp_cgi_add();
  }
  elseif ($action == 'touch') {
    new yp_cgi_touch();
  }
  elseif ($action == 'remove') {
    new yp_cgi_remove();
  }
}

class yp_cgi {

  function yp_cgi() {
    foreach ($this->map as $key => $variables) {
      $this->$key = '';
      foreach ($variables as $variable) {
        if (isset($_POST[$variable])) {
          $this->$key = trim($_POST[$variable]);
        }
      }
    }
    $this->exec();
  }

}

class yp_cgi_add extends yp_cgi {

  var $map = array(
    'server_name' => array('sn'),
    'server_type' => array('type'),
    'genre' => array('genre'),
    'bitrate' => array('audio_bitrate', 'b', 'bitrate', 'ice-bitrate'),
    'samplerate' => array('audio_samplerate', 'samplerate', 'ice-samplerate'),
    'channels' => array('audio_channels', 'channels', 'ice-channels'),
    'listen_url' => array('listenurl'),
    'description' => array('desc'),
    'url' => array('url'),
  );

  function exec() { 
    $listing_ip = ip_address();
    db_query("DELETE FROM {yp_stream} WHERE listing_ip = '%s' AND listen_url = '%s'", $listing_ip, $this->listen_url);
    db_query("INSERT INTO {yp_stream} (server_name, server_type, genre, bitrate, listen_url, description, url, listing_ip, last_touch, channels, samplerate) 
      VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s')", $this->server_name, $this->server_type, $this->genre, $this->bitrate, 
      $this->listen_url, $this->description, $this->url, $listing_ip, time(), $this->channels, $this->samplerate);
    $sid = db_last_insert_id('yp_stream', 'sid');
    $yp_response = $sid ? 1 : 0;
    header("SID: $sid");
    header('TouchFreq: 200');
    header('YPMessage: Added');
    header("YPResponse: $yp_response");
    db_query("DELETE FROM {yp_stream} WHERE %d - last_touch > 300", time());
  }

}

class yp_cgi_touch extends yp_cgi {

  var $map = array(
    'sid' => array('sid'),
    'listeners' => array('listeners'),
    'max_listeners' => array('max_listeners'),
    'server_subtype' => array('stype'),
    'current_song' => array('st'),
  );

  function exec() { 
    db_query("UPDATE {yp_stream} SET listeners = %d, max_listeners = %d, server_subtype = '%s', current_song = '%s', last_touch = %d WHERE sid = %d",
      $this->listeners, $this->max_listeners, $this->server_subtype, $this->current_song, time(), $this->sid);
    if (db_affected_rows()) {
      $yp_response = 1;
      $yp_message = 'Touched';
    }
    else {
      $yp_response = 0;
      $yp_message = 'SID not found';
    }
    header("YPResponse: $yp_response");
    header("YPMessage: $yp_message");
  }

}

class yp_cgi_remove extends yp_cgi {

  var $map = array(
    'sid' => array('sid'),
  );

  function exec() { 
    db_query("DELETE FROM {yp_stream} WHERE sid = %d", $this->sid);
    header('YPResponse: 1');
    header('YPMessage: Removed');
  }

}
