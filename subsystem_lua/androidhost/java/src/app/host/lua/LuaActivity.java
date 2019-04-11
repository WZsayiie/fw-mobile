package src.app.host.lua;

import android.app.Activity;
import android.os.Bundle;

import java.io.File;

import src.library.foundation.L;

public class LuaActivity extends Activity {

    static {
        System.loadLibrary("cargo");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        L.i("host: on create");

        String cachePath= getCacheDir().toString();
        File luaPath = new File(cachePath + "/LUA");
        luaPath.mkdir();

        handleCreate(luaPath.getPath());
    }

    @Override
    protected void onStart() {
        super.onStart();
        L.i("host: on start");

        handleStart();
    }

    @Override
    protected void onStop() {
        super.onStop();
        L.i("host: on stop");

        handleStop();
    }

    private native void handleCreate(String path);
    private native void handleStart();
    private native void handleStop();
}
