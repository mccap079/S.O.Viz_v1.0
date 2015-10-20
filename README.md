# mapsat

*(c) 2015 Andrew McCausland*

Made with [OF v.0.8.3](http://openframeworks.cc/download/older.html) on OSX 10.8.5 with XCode 5.1.1.

This is a work in progress.

##The Data

The data used in the visualization is retrieved via the [space-track.org](https://www.space-track.org/auth/login) API and stored in a local JSON file. Every time the app is run, the latest [TLE](https://en.wikipedia.org/wiki/Two-line_element_set) data is retrieved, and overwrites the existing local data. 

**Note:** There's currently a dataset included at `/bin/data/spaceTrackQuery.json` so that the time-consuming data-retrieval process (+15,000 objects) may be skipped for debugging purposes. If you'd like to update to the latest dataset, simply uncomment the `system(cmd);` line from within ofApp.cpp before running the app.
