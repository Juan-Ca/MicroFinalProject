//
// Copyright 2017 Amazon.com, Inc. or its affiliates (Amazon). All Rights Reserved.
//
// Code generated by AWS Mobile Hub. Amazon gives unlimited permission to 
// copy, distribute and modify it.
//
// Source code generated from template: aws-my-sample-app-android v0.16
//
package com.mysampleapp;

import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBar;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;

import com.amazonaws.mobile.AWSMobileClient;
import com.amazonaws.mobilehelper.auth.IdentityManager;
import com.amazonaws.mobileconnectors.cognito.Dataset;
import com.amazonaws.mobileconnectors.cognito.DefaultSyncCallback;
import com.amazonaws.mobileconnectors.cognito.Record;
import com.mysampleapp.demo.DemoConfiguration;
import com.mysampleapp.demo.HomeDemoFragment;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.support.v4.content.LocalBroadcastManager;

import com.mysampleapp.navigation.NavigationDrawer;
import com.mysampleapp.demo.UserSettings;
import java.util.List;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    /** Class name for log messages. */
    private static final String LOG_TAG = MainActivity.class.getSimpleName();

    /** Bundle key for saving/restoring the toolbar title. */
    private static final String BUNDLE_KEY_TOOLBAR_TITLE = "title";

    /** The identity manager used to keep track of the current user account. */
    private IdentityManager identityManager;

    /** The toolbar view control. */
    private Toolbar toolbar;

    /** Our navigation drawer class for handling navigation drawer logic. */
    private NavigationDrawer navigationDrawer;

    /** The helper class used to toggle the left navigation drawer open and closed. */
    private ActionBarDrawerToggle drawerToggle;

    /** Data to be passed between fragments. */
    private Bundle fragmentBundle;

    /**
     * Initializes the Toolbar for use with the activity.
     */
    private void setupToolbar(final Bundle savedInstanceState) {
        toolbar = (Toolbar) findViewById(R.id.toolbar);
        // Set up the activity to use this toolbar. As a side effect this sets the Toolbar's title
        // to the activity's title.
        setSupportActionBar(toolbar);

        if (savedInstanceState != null) {
            // Some IDEs such as Android Studio complain about possible NPE without this check.
            assert getSupportActionBar() != null;

            // Restore the Toolbar's title.
            getSupportActionBar().setTitle(
                savedInstanceState.getCharSequence(BUNDLE_KEY_TOOLBAR_TITLE));
        }
    }


    /**
     * Initializes the navigation drawer menu to allow toggling via the toolbar or swipe from the
     * side of the screen.
     */
    private void setupNavigationMenu(final Bundle savedInstanceState) {
        final DrawerLayout drawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
        final ListView drawerItems = (ListView) findViewById(R.id.nav_drawer_items);

        // Create the navigation drawer.
        navigationDrawer = new NavigationDrawer(this, toolbar, drawerLayout, drawerItems,
            R.id.main_fragment_container);

        // Add navigation drawer menu items.
        // Home isn't a demo, but is fake as a demo.
        DemoConfiguration.DemoFeature home = new DemoConfiguration.DemoFeature();
        home.iconResId = R.mipmap.icon_home;
        home.titleResId = R.string.main_nav_menu_item_home;
        navigationDrawer.addDemoFeatureToMenu(home);

        for (DemoConfiguration.DemoFeature demoFeature : DemoConfiguration.getDemoFeatureList()) {
            navigationDrawer.addDemoFeatureToMenu(demoFeature);
        }

        if (savedInstanceState == null) {
            // Add the home fragment to be displayed initially.
            navigationDrawer.showHome();
        }
    }

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Obtain a reference to the mobile client. It is created in the Application class,
        // but in case a custom Application class is not used, we initialize it here if necessary.
        AWSMobileClient.initializeMobileClientIfNecessary(this);

        // Obtain a reference to the mobile client. It is created in the Application class.
        final AWSMobileClient awsMobileClient = AWSMobileClient.defaultMobileClient();

        // Obtain a reference to the identity manager.
        identityManager = awsMobileClient.getIdentityManager();

        setContentView(R.layout.activity_main);

        setupToolbar(savedInstanceState);

        setupNavigationMenu(savedInstanceState);
    }

    @Override
    protected void onResume() {
        super.onResume();

        // register settings changed receiver.
        LocalBroadcastManager.getInstance(this).registerReceiver(settingsChangedReceiver,
            new IntentFilter(UserSettings.ACTION_SETTINGS_CHANGED));
        updateColor();
    }

    @Override
    public boolean onOptionsItemSelected(final MenuItem item) {
        // Handle action bar item clicks here excluding the home button.

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onSaveInstanceState(final Bundle bundle) {
        super.onSaveInstanceState(bundle);
        // Save the title so it will be restored properly to match the view loaded when rotation
        // was changed or in case the activity was destroyed.
        if (toolbar != null) {
            bundle.putCharSequence(BUNDLE_KEY_TOOLBAR_TITLE, toolbar.getTitle());
        }
    }

    @Override
    public void onClick(final View view) {
    }
    
    private final BroadcastReceiver settingsChangedReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.d(LOG_TAG, "Received settings changed local broadcast. Update theme colors.");
            updateColor();
        }
    };

    @Override
    protected void onPause() {
        super.onPause();

        LocalBroadcastManager.getInstance(this).unregisterReceiver(settingsChangedReceiver);
    }

    @Override
    public void onBackPressed() {
        final FragmentManager fragmentManager = this.getSupportFragmentManager();
        
        if (navigationDrawer.isDrawerOpen()) {
            navigationDrawer.closeDrawer();
            return;
        }

        if (fragmentManager.getBackStackEntryCount() == 0) {
            if (fragmentManager.findFragmentByTag(HomeDemoFragment.class.getSimpleName()) == null) {
                final Class fragmentClass = HomeDemoFragment.class;
                // if we aren't on the home fragment, navigate home.
                final Fragment fragment = Fragment.instantiate(this, fragmentClass.getName());

                fragmentManager
                    .beginTransaction()
                    .replace(R.id.main_fragment_container, fragment, fragmentClass.getSimpleName())
                    .setTransition(FragmentTransaction.TRANSIT_FRAGMENT_OPEN)
                    .commit();

                // Set the title for the fragment.
                final ActionBar actionBar = this.getSupportActionBar();
                if (actionBar != null) {
                    actionBar.setTitle(getString(R.string.app_name));
                }
                return;
            }
        }
        super.onBackPressed();
    }

    public void updateColor() {
        final UserSettings userSettings = UserSettings.getInstance(getApplicationContext());
        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(final Void... params) {
                userSettings.loadFromDataset();
                return null;
            }

            @Override
            protected void onPostExecute(final Void aVoid) {
                toolbar.setTitleTextColor(userSettings.getTitleTextColor());
                toolbar.setBackgroundColor(userSettings.getTitleBarColor());
                final Fragment fragment = getSupportFragmentManager().findFragmentById(R.id.main_fragment_container);
                if (fragment != null) {
                    final View fragmentView = fragment.getView();
                    if (fragmentView != null) {
                        fragmentView.setBackgroundColor(userSettings.getBackgroudColor());
                    }
                }
            }
        }.execute();
    }


    /**
     * Stores data to be passed between fragments.
     * @param fragmentBundle fragment data
     */
    public void setFragmentBundle(final Bundle fragmentBundle) {
        this.fragmentBundle = fragmentBundle;
    }

    /**
     * Gets data to be passed between fragments.
     * @return fragmentBundle fragment data
     */
    public Bundle getFragmentBundle() {
        return this.fragmentBundle;
    }
}
