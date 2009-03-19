<?php
// $Id$

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
  $form['yplog_pear'] = array(
    '#type' => 'textfield',
    '#title' => t('PEAR directory'),
    '#description' => t('If PEAR libraries are not in the PHP include path, you must enter a path here, including trailing slash. For example, enter <em>./sites/all/pear/</em> if the Image_Graph libary is at <em>sites/all/pear/Image/Graph.php</em> or <em>/usr/local/share/pear/</em> if the Image_Graph library is at <em>/usr/local/share/pear/Image/Graph.php</em>.'),
    '#default_value' => variable_get('yplog_pear', ''),
  );
  return system_settings_form($form);
}
