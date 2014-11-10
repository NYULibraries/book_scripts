<?php

/**
 * A command-line Drupal script to upadte DLTS Image
 */

function dlts_image_field_schema_7001($field) {
  field_cache_clear();
  return array(
      'columns' => array(
        'fid' => array(
          'description' => 'The {file_managed}.fid being referenced in this field.',
          'type' => 'int',
          'not null' => FALSE,
          'unsigned' => TRUE,
        ),
        'djakota_width' => array(
          'description' => 'The width of the image in pixels.',
          'type' => 'int',
          'unsigned' => TRUE,
        ),
        'djakota_height' => array(
          'description' => 'The height of the image in pixels.',
          'type' => 'int',
          'unsigned' => TRUE,
        ),
        'djakota_levels' => array(
          'description' => 'Djatoka Jpeg 2000 Image Server levels values of the image.',
          'type' => 'int',
          'unsigned' => TRUE,
        ),
        'djakota_dwtLevels' => array(
          'description' => 'Djatoka Jpeg 2000 Image Server dwtLevels values of the image.',
          'type' => 'int',
          'unsigned' => TRUE,
        ),
        'djakota_compositingLayerCount' => array(
          'description' => 'Djatoka Jpeg 2000 Image Server compositing layer count of the image.',
          'type' => 'int',
          'unsigned' => TRUE,
        ),
        'width' => array(
          'description' => 'The width of the image in pixels.',
          'type' => 'int',
          'unsigned' => TRUE,
        ),
        'height' => array(
          'description' => 'The height of the image in pixels.',
          'type' => 'int',
          'unsigned' => TRUE,
        ),
      ),
      'indexes' => array(
        'fid' => array('fid'),
      ),
      'foreign keys' => array(
        'fid' => array(
          'table' => 'file_managed',
          'columns' => array('fid' => 'fid'),
        ),
      ),
    );
}

function change_dlts_image_field_config_instance($field_name) {
  field_cache_clear();

  $result = db_query('SELECT * FROM {field_config_instance} WHERE field_name = :field_name', array(':field_name' => $field_name));
  $record = $result->fetchObject();  
  $old_data = unserialize($record->data);
  $data = serialize(
    array(
      'label' => $old_data['label'],
      'widget' => array(
        'weight' => $old_data['widget']['weight'],
        'type' => 'dlts_image',
        'module' => 'dlts_image',
        'active' => 1,
        'settings' => array(
          'progress_indicator' => 'throbber',
          'preview_image_style' => 'thumbnail',
        ),
      ),
      'settings' => array(
        'file_directory' => '',
        'max_filesize' => '', 
        'file_extensions' => 'jp2 tif tiff jpg jpeg',
        'user_register_form' => '',
      ),
      'display' => $old_data['label']['display'],
      'required' => $old_data['required'],
      'description' => '',
    )
  ); 
  
  drush_print($data);

  db_update('field_config_instance')->fields(
    array(
      'data' => $data
      )
    )
    ->condition('field_name', $field_name, '=')
    ->execute();
    
  field_cache_clear();
}

function change_dlts_image_field_config($field_name) {
  field_cache_clear();
  $result = db_query('SELECT id, data FROM {field_config} WHERE field_name = :field_name', array(':field_name' => $field_name));
  $record = $result->fetchObject();
  $data = serialize(
    array(
      'translatable' => '0',
      'entity_types' => array(),
      'settings' => array(
        'uri_scheme' => 'public',
        'default_image' => 0,
      ),
      'storage' => array(
        'type' => 'field_sql_storage',
        'settings' => array(),
        'module' => 'field_sql_storage',
        'active' => '1',
        'details' => array(
          'sql' => array(
            'FIELD_LOAD_CURRENT' => array(
              'field_data_' . $field_name => array(
                'fid' => $field_name . '_fid',
                'djakota_width' => $field_name . '_djakota_width',
                'djakota_height' => $field_name . '_djakota_height',
                'djakota_levels' => $field_name . '_djakota_levels',
                'djakota_dwtLevels' => $field_name . '_djakota_dwtLevels',
                'djakota_compositingLayerCount' => $field_name . '_djakota_compositingLayerCount',
                'width' => $field_name . '_width',
                'height' => $field_name . '_height',
              ),
            ),
            'FIELD_LOAD_REVISION' => array(
              'field_revision_' . $field_name => array(
                'fid' => $field_name . '_fid',
                'djakota_width' => $field_name . '_djakota_width',
                'djakota_height' => $field_name . '_djakota_height',
                'djakota_levels' => $field_name . '_djakota_levels',
                'djakota_dwtLevels' => $field_name . '_djakota_dwtLevels',
                'djakota_compositingLayerCount' => $field_name . '_compositingLayerCount',
                'width' => $field_name . '_width',
                'height' => $field_name . '_height',
              ),
            ),
          ),
        ),
      ),
      'foreign keys' => array(
        'fid' => array(
          'table' => 'file_managed',
          'columns' => array(
          'fid' => 'fid',
        ),
      ),
    ),
    'indexes' => array(
      'fid' => array(
        '0' => 'fid',
      ),
    ),
    'id' => ''. $record->id .'',
    )
  );
  
  db_update('field_config')->fields(
    array(
      'type' => 'dlts_image',
      'module' => 'dlts_image',
      'data' => $data
      )
    )
    ->condition('field_name', $field_name, '=')
    ->execute();

  field_cache_clear();
}

function change_dlts_image_fields() {
  field_cache_clear();
  
  $fields = field_info_fields();
  $change_config = array();
  foreach ($fields as $field_name => $field) {
    if ($field['type'] == 'image' && $field['storage']['type'] == 'field_sql_storage' && ( $field_name == 'field_cropped_master' || $field_name == 'field_stitch_image' ) ) {
      $change_config[] = $field_name;
      $schema = dlts_image_field_schema_7001($field);
      foreach ($field['storage']['details']['sql'] as $type => $table_info) {
        foreach ($table_info as $table_name => $columns) {
          $column_name = _field_sql_storage_columnname($field_name, 'djakota_width');
          db_add_field($table_name, $column_name, $schema['columns']['djakota_width']);
          $column_name = _field_sql_storage_columnname($field_name, 'djakota_height');
          db_add_field($table_name, $column_name, $schema['columns']['djakota_height']);
          $column_name = _field_sql_storage_columnname($field_name, 'djakota_levels');
          db_add_field($table_name, $column_name, $schema['columns']['djakota_levels']);
          $column_name = _field_sql_storage_columnname($field_name, 'djakota_dwtLevels');
          db_add_field($table_name, $column_name, $schema['columns']['djakota_dwtLevels']);
          $column_name = _field_sql_storage_columnname($field_name, 'djakota_compositingLayerCount');
          db_add_field($table_name, $column_name, $schema['columns']['djakota_compositingLayerCount']);
          $column_name = _field_sql_storage_columnname($field_name, 'alt');
          db_drop_field($table_name, $column_name);
          $column_name = _field_sql_storage_columnname($field_name, 'title');
          db_drop_field($table_name, $column_name);
        }
      }
    }
  }
  
  foreach ($change_config as $field_name) {  
    change_dlts_image_field_config($field_name);
    change_dlts_image_field_config_instance($field_name);
  }
  
  field_cache_clear();
}

function _dlts_image_fields_content_populate_metadata($table, $field_name) {

  field_cache_clear();
  
  $count = db_select($table)->countQuery()->execute()->fetchField();
  
  if (!$count) {
    drush_print('Nothing to do here');
    return;
  }
  
  $query = db_select($table, NULL, array('fetch' => PDO::FETCH_ASSOC));
  $query->join('file_managed', NULL, $table . '.' . $field_name . '_fid = file_managed.fid');
  $result = $query->fields('file_managed', array('fid', 'uri'))->orderBy('file_managed.fid')->execute();  
  $i = 0;
  
  foreach ($result as $file) {
    $i++;
    $uri = '';
    $info = dlts_image_djatoka_request($file);
    
    if (is_array($info)) {
      drush_print('Updating: '. $i . ' out of ' . $count . '. uri: ' . $info['identifier'] . ' ( fid: ' . $file['fid'] . ')');
      drush_print_r($info);
      drush_print("\n");  

      db_update($table)
        ->fields(array(
          $field_name . '_djakota_width' => $info['width'],
          $field_name . '_djakota_height' => $info['height'],
          $field_name . '_djakota_dwtLevels' => $info['dwtLevels'],
          $field_name . '_djakota_levels' => $info['levels'],
          $field_name . '_djakota_compositingLayerCount' => $info['compositingLayerCount'],          
        ))
        ->condition($field_name . '_fid', $file['fid'])
        ->execute();

    }
  }  
  field_cache_clear();  
}

function update_dlts_image_fields_content() {
  field_cache_clear();
  
  $fields = field_read_fields(array(
    'module' => 'dlts_image',
    'storage_type' => 'field_sql_storage',
  ));

  foreach ($fields as $field_name => $field) {
    $tables = array(
      _field_sql_storage_tablename($field),
      _field_sql_storage_revision_tablename($field),
    );
    foreach ($tables as $table) {
      drush_print($table);
      drush_print("\n");
      _dlts_image_fields_content_populate_metadata($table, $field_name);      
    }
  }  
  field_cache_clear();
}

function dlts_image_fields_update_run() {
  // change_dlts_image_fields();
update_dlts_image_fields_content();
}

dlts_image_fields_update_run();
