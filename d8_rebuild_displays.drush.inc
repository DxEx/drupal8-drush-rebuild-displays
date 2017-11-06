<?php

/**
 * Implementation of hook_drush_command().
 */
function d8_rebuild_displays_drush_command() {
  $items = [];
  $items['d8-rebuild-displays'] = [
    'callback' => 'drush_rebuild_displays',
    'description' => dt('Rebuilds all view- and form-displays.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  ];

  return $items;
}

/**
 * Load all displays of all entities. Rebuild and save them.
 */
function drush_rebuild_displays() {

  // Loop through all entity types.
  foreach (\Drupal::entityTypeManager()->getDefinitions() as $entity_type_id => $entity_type_definition) {

    // Loop through all bundles.
    foreach (entity_get_bundles($entity_type_id) as $bundle => $bundle_definition) {

      // Get the displays.
      $entity_display_repository = \Drupal::service('entity_display.repository');
      $view_modes = $entity_display_repository->getViewModes($entity_type_id);
      $view_modes['default'] = ['status' => TRUE];
      $form_modes = $entity_display_repository->getFormModes($entity_type_id);
      $form_modes['default'] = ['status' => TRUE];

      // Update view modes.
      foreach ($view_modes as $view_mode_id => $view_mode) {
        if ($view_mode['status']) {
          if ($display = \Drupal\Core\Entity\Entity\EntityViewDisplay::load($entity_type_id . '.' . $bundle . '.' . $view_mode_id)) {
            $display->save();
            drush_print("Rebuild view display: $entity_type_id.$bundle.$view_mode_id");
          }
        }
      }

      // Update form modes.
      foreach ($form_modes as $form_mode_id => $form_mode) {
        if ($form_mode['status']) {
          if ($form_display = \Drupal\Core\Entity\Entity\EntityFormDisplay::load($entity_type_id . '.' . $bundle . '.' . $form_mode_id)) {
            $form_display->save();
            drush_print("Rebuild form display: $entity_type_id.$bundle.$form_mode_id");
          }
        }
      }
    }

  }

}
