<?php

/**
 * Page callback for settings.
 */
function yplog_settings() {
  $form['yplog_gmdate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use GMT on graph X axis'),
    '#description' => t('If unchecked, graph labels will use local time of the current user.'),
    '#default_value' => variable_get('yplog_gmdate', 1),
  );
  $form['yplog_font'] = array(
    '#type' => 'textfield',
    '#title' => t('Graph font'),
    '#description' => t('Enter the path to a font file on the server file system.'),
    '#default_value' => variable_get('yplog_font', '/usr/local/lib/X11/fonts/webfonts/verdana.ttf'),
  );
  return system_settings_form($form);
}
