<?php
// $Id$

function yp_cgi() {
  if (variable_get('yp_debug', 0)) {
    watchdog('yp', check_plain(print_r($_POST, TRUE)), NULL, WATCHDOG_DEBUG);
  }
  $action = isset($_POST['action']) ? trim($_POST['action']) : '';
  switch ($action) {
    case 'add':
      new YPCGIAdd();
      break;
    case 'touch':
      new YPCGITouch();
      break;
    case 'remove':
      new YPCGIRemove();
      break;
  }
}

class YPCGI {

  function YPCGI() {
    $this->receive();
    $this->process();
    $this->response();
  }

  function receive() {
    foreach ($this->map as $key => $variables) {
      $this->$key = '';
      foreach ($variables as $variable) {
        if (isset($_POST[$variable])) {
          $this->$key = trim($_POST[$variable]);
        }
      }
    }
  }

}

class YPCGIAdd extends YPCGI {

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

  function process() { 
    db_query("DELETE FROM {yp_stream} WHERE listing_ip = '%s' AND listen_url = '%s'", ip_address(), $this->listen_url);
    db_query("INSERT INTO {yp_stream} (server_name, server_type, genre, bitrate, listen_url, description, url, listing_ip, last_touch, channels, samplerate) 
      VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s')", $this->server_name, $this->server_type, $this->genre, $this->bitrate, 
      $this->listen_url, $this->description, $this->url, ip_address(), time(), $this->channels, $this->samplerate);
    $this->sid = db_last_insert_id('yp_stream', 'sid');
    $this->yp_response = $this->sid ? 1 : 0;
    $this->yp_message = $this->response ? 'Added' : 'Error';
  }

  function response() {
    header("SID: $this->sid");
    header('TouchFreq: 200');
    header('YPMessage: $this->yp_message');
    header("YPResponse: $this->yp_response");
    db_query("DELETE FROM {yp_stream} WHERE %d - last_touch > 300", time());
  }
}

class YPCGITouch extends YPCGI {

  var $map = array(
    'sid' => array('sid'),
    'listeners' => array('listeners'),
    'max_listeners' => array('max_listeners'),
    'server_subtype' => array('stype'),
    'current_song' => array('st'),
  );

  function process() { 
    db_query("UPDATE {yp_stream} SET listeners = %d, max_listeners = %d, server_subtype = '%s', current_song = '%s', last_touch = %d WHERE sid = %d",
      $this->listeners, $this->max_listeners, $this->server_subtype, $this->current_song, time(), $this->sid);
    $this->yp_response = db_affected_rows() ? 1 : 0;
    $this->yp_message = $this->yp_response ? 'Touched' : 'SID not found';
  }

  function response() {
    header("YPResponse: $this->yp_response");
    header("YPMessage: $this->yp_message");
  }

}

class YPCGIRemove extends YPCGI {

  var $map = array(
    'sid' => array('sid'),
  );

  function process() { 
    db_query("DELETE FROM {yp_stream} WHERE sid = %d", $this->sid);
  }

  function response() {
    header('YPResponse: 1');
    header('YPMessage: Removed');
  }

}
