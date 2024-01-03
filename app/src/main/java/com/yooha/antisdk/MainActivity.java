package com.yooha.antisdk;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.app.ActivityManager;
import android.app.Application;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.yooha.antisdk.tools.XposedUtil;
import com.yooha.antisdk.tools.YoohaUtil;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Type;
import java.lang.reflect.TypeVariable;
import java.util.ArrayList;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class MainActivity extends AppCompatActivity {
    public String mTag = "yooha-anti-java";
    public MainActivity mThiz = null;
    public EditText mEtx;
    public Button mBtn;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mEtx = findViewById(R.id.editTextTextPassword);
        mBtn = findViewById(R.id.button);

//        Log.i("yooha-anti-ndk123", Environment.getDataDirectory().getPath());
//        Log.i("yooha-anti-ndk123", Environment.getDataDirectory().toString());

        mThiz = this;
        YoohaUtil.init();
        mBtn.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {



                //XposedUtil.is_load_xposed_dex(getBaseContext().getClassLoader());
                //XposedUtil.is_load_xposed_class();

                String key = mEtx.getText().toString();
                Log.i(mTag, "onClick key -> " + key);
                YoohaUtil.check(key, mThiz);
            }
        });

    }

    @RequiresApi(api = Build.VERSION_CODES.P)
    public void test() {
//        try{
//            Field[] fs = PackageInfo.class.getFields();
//            for (Field f:fs){
//                Log.i("abcabc", f.getName());
//                Log.i("abcabc", f.toString());
//                Log.i("abcabc", f.toGenericString());
////            Log.i("abcabc", f.getName());
//            }
//        }catch(Throwable th){
//
//        }
//        Application ap;
//        Context con;
//        List li;
        //Application.class
//        ArrayList as = null;
//        as.size();
//        as.get(1);
        


        Log.i("testtest", "in test");
        Method[] mes = ClipboardManager.class.getDeclaredMethods();
        for (Method me:mes){
            //TypeVariable<Method>[] ty = me.getTypeParameters();
            Type[] type = me.getGenericParameterTypes();
            for (Type t:type){
                Log.i("testtest", me.getName() + " -> " + t.getTypeName());
            }

        }
    }



    public void passTo(){
        Log.i(mTag, "in Java passTo");
        Intent intent = new Intent(MainActivity.this, PassActivity.class);
        intent.putExtra("value", YoohaUtil.getPassValue());
        startActivity(intent);
    }




}