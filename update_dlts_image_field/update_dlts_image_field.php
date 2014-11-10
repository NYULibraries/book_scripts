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

  $result = db_query('SELECT data FROM {field_config_instance} WHERE field_name = :field_name', array(':field_name' => $field_name));
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
      'display' => $old_data['display'],
      'required' => $old_data['required'],
      'description' => '',
    )
  ); 
   
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
 
  $id = "";

  if (func_num_args() > 2){
   $id=func_get_arg(2);
  }
 
  field_cache_clear();
  
  $count = db_select($table)->countQuery()->execute()->fetchField();
  
  if (!$count) {
    drush_print('Nothing to do here');
    return;
  }
  
  $query = db_select($table, NULL, array('fetch' => PDO::FETCH_ASSOC));
 
  $query->join('file_managed', NULL, $table . '.' . $field_name . '_fid = file_managed.fid');
  
  if (!(empty($id))){
    $query->condition('file_managed.filename',  $id . '%','LIKE'); 
  }

  $result = $query->fields('file_managed', array('fid', 'uri'))->orderBy('file_managed.fid')->execute();  
  
  $i = 0;
  
  foreach ($result as $file) {
    $i++;
    
    $info = dlts_image_djatoka_request(file_load($file['fid']));
 
    if (is_array($info)) {
      drush_print('Updating: '. $i . ' out of ' . $count . '. Current fid: ' . $file['fid']);
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
  $id = "";
  if (func_num_args() > 0){
   $id=func_get_arg(0);
  }
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
      if (empty($id)){
        _dlts_image_fields_content_populate_metadata($table, $field_name);      
      }else{
        _dlts_image_fields_content_populate_metadata($table, $field_name,$id);      
     }
    }
  }  
  field_cache_clear();
}

function delete_all_dlts_books_book_path_alias() {
  $results = db_query('SELECT n.nid FROM {node} n WHERE type = :type', array(':type' => 'dlts_book'));  
  foreach ( $results as $key => $result ) {    
    db_delete('url_alias')->condition('source', 'node/' . $result->nid)->execute();
  }
}

/*
 * Get the first page off all the books and create a new path alias
 */
function add_book_alias_to_dlts_books_page() {
  $results = db_query('
    SELECT 
      n.nid,
      i.field_is_part_of_value
    FROM {node} n
    LEFT JOIN {field_data_field_sequence_number} s ON n.nid = s.entity_id
    LEFT JOIN {field_data_field_is_part_of} i ON n.nid = i.entity_id
    WHERE type = :type AND s.field_sequence_number_value = 1
  ', array(':type' => 'dlts_book_page')); 

  foreach ( $results as $key => $result ) {
      drush_print('Inserting alias books/' . $result->field_is_part_of_value . ' to node/' . $result->nid );
      db_insert('url_alias')->fields(
        array(
          'source' => 'node/' . $result->nid,
          'alias' => 'books/' . $result->field_is_part_of_value,
          'language' => 'en',
      )
    )
    ->execute();
    
  }
}           
  
function dlts_image_fields_update_run($task) {
  global $base_url;
  
  // Test if $base_url has been set in settings.php
  if ( !preg_match( '/\.edu\.+/', $base_url ) ) {
    
    switch ($task) {
      
      case 1 :
        
        /** Do all at once */
        drush_print('Running: Path delete dlts_books alias');
        delete_all_dlts_books_book_path_alias();
        drush_print('Running: Add alias to page 1 of each dlts_book');
        add_book_alias_to_dlts_books_page();
        drush_print('Changing DLTS Image fields configuration');
        change_dlts_image_fields();
        drush_print('Done updating fields');
        update_dlts_image_fields_content();
        break;

      case 2 :
        
          drush_print('Running: Path delete dlts_books alias');
          delete_all_dlts_books_book_path_alias();
          drush_print('Running: Add alias to page 1 of each dlts_book');
          add_book_alias_to_dlts_books_page();
        break;
        
      case 3 :
        
        drush_print('Using ' . variable_get('dlts_image_djatoka_service', 'NULL') . ' as Djatoka Image server request URL'); 

        $djatoka = dlts_image_djatoka_request(array( 'uri' => drupal_get_path('module', 'dlts_image') . '/img/caspar.jpg'));

        if (!isset($djatoka['error'])) {
          drush_print('Changing DLTS Image fields configuration');
          change_dlts_image_fields();
          drush_print('Done updating fields');
        }
        else {
          drush_print('Unable to reach Djatoka Image server');
        }
        
        break;
        
      case 4 :

        update_dlts_image_fields_content();
        break;
        
      default : 
        drush_print('');
        drush_print('ERROR: Unable to perform task');
        drush_print('');
        show_options();
        break;
      
    }
  }
  else {
    drush_print('Before runnint this script $base_url must be set in settings.php');
  }
}

function show_help() {
  drush_print('');
  drush_print(' To run the script: drush scr update_dlts_image_field.php 1');
  drush_print(' To run the script to update DLTS image field content by id: drush scr update_dlts_image_field.php 5 mss100_ref20');
  drush_print(' A list of ids can also be sent as a file: update_dlts_image_field.php  5 id.txt');
  drush_print(' The following options can be used:');
  drush_print('[1] Run all the tasks');
  drush_print('[2] Update alias');
  drush_print('[3] Update DLTS image field config and field instances');
  drush_print('[4] Update all DLTS image field content');
  drush_print('[5] Update  DLTS image field content by specified id');
}

function show_options() {
  show_help();
}

function get_id($id){
   if (is_file($id)){

      $handle = @fopen($id, "r") or exit("unable to open file ($id)");
      if ($handle) {
          while (($line = fgets($handle, 4096)) !== false) {
             update_dlts_image_fields_content(trim($line));
             //sleep(30);
          }
          if (!feof($handle)) {
              echo "Error: unexpected fgets() fail\n";
          }
          fclose($handle);
      }
   }else{
       update_dlts_image_fields_content($id);
   }
}
/** Init */
$no_v_params=array("1","2","3","4");  //acceptable params with no accompanying value
$md_by_id="5";   //param to send id
$option=$args[1];
$count = count($args);
switch($count){
  case 2:
     if (in_array($option,$no_v_params,true)){
        dlts_image_fields_update_run($option);
     } else{
        show_options();
     }
     break;
  case 3:
     if ($option == $md_by_id) {
        $id = $args[2];
        get_id($id);
     }
     else{
        show_options();
     }
     break;
  default:
    show_options();
}

