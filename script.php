<?php

/**
 * A command-line Drupal script to create a dummy book
 *
 * drush php-script script.php --user=1 --uri=http://dev-dl-pa.home.nyu.edu/books
 */
 
function show_help(array $commands = array()) {

  drush_print('');

  foreach ($commands as $key => $option) {
    drush_print('[' . $key . '] ' . $option['label']);
  }

  drush_print('');

}

function show_options (array $commands = array()) {

  drush_print(t('Please type one of the following options to continue:'));

  show_help($commands);

  $handle = fopen ("php://stdin","r");

  $line = fgets($handle);

  run(trim($line), $commands);

}

function run ($task, array $commands = array()) {
  if (isset($commands[$task]) && isset($commands[$task]['callback'])) {
    foreach ($commands[$task]['callback'] as $callback) {
      if ( function_exists ( $callback ) ) $callback() ;
    }
  }
  else {
    drush_log ( dt( 'ERROR: Unable to perform @task', array ( '@task' => $task ) ), 'error' ) ;
    show_options($commands);
  }
}

function init ( array $options = array() ) {

  ini_set('memory_limit', '512MM');

  include_once('inc/common.inc');

  include_once('inc/commands/default.inc');
  
  include_once('inc/datasource/default.inc');
  
  include_once('inc/content/default.inc');

  $caller = (isset($trace[0]['file']) ? $trace[0]['file'] : __FILE__);

  $default_commands = default_commands();

  $commands = array_merge($default_commands, $options['commands']);

  // Add exit command to end of the commands options  
  $commands[] = array(
    'label' => t('Exit'),
    'callback' => array(
      'drush_exit',
    ),
  );
  
  if ( $task = drush_get_option('task') ) {
    
    drush_log( dt( 'Loading task "@task"' , array( '@task' => $task ) ), 'status') ;
        
    foreach ( $commands as $key => $command ) if ( $commands[$key]['label'] == $task ) $action = $key ;

    if ( $action ) run ( $action, $commands ) ;

    else drush_log('Unable to load task', 'error') ;

  }  
  
  else {
  
    if ( isset( $args[1])  ) run( $args[1], $commands ) ;
  
    else  show_options( $commands ) ;
  
  }

}

init( array( 'commands' => array() ) );