<?php
// $Id$

function template_preprocess_yplog_graph(&$variables) {
  $variables['image'] = theme('image', $variables['filename']);
  $variables['listen_link'] = l($variables['listen_url'] . '.m3u', $variables['listen_url'] . '.m3u');
}
