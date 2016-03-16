<?php

/** list of commands */

function DLTSACO_304_commands() {
  $commands = array(
    array(
      'label' => t('Fix wrong  PDFs label'),
      'callback' => array(
        'DLTSACO_304_fix_pdf_wrong_labels',
      ),
    ),
  );
  return $commands;
}

/**
 * PDF: high resolution, Low resolution is being replaced by links is come cases and on both English and Arabic sides.
 * https://jira.nyu.edu/browse/DLTSACO-304
 */
function DLTSACO_304_fix_pdf_wrong_labels() {
	$data = serialize(array());
  foreach (books() as $book) {
  	$wrapper = entity_metadata_wrapper('node', node_load($book));
  	drush_log($wrapper->label(),'ok');
  	$values = $wrapper->field_pdf_file->value();
  	$pdf_files = array();

  	if (!empty($values)) {
  		foreach ($values as $index => $value) {
  			$values[$index]['data'] = $data;
        if (strpos($value['uri'], '_hi')) {
        	$values[$index]['label'] = 'High resolution';
        }
        elseif (strpos($value['uri'], '_lo')) {
        	$values[$index]['label'] = 'Low resolution';
        }
  		}
  	}
  	$wrapper->field_pdf_file->set($values);
  	$wrapper->save();
  	unset($value);
  	unset($values);
  	unset($wrapper);
  }
}
