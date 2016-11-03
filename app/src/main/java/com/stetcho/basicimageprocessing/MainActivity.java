package com.stetcho.basicimageprocessing;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.view.ViewGroup;
import android.widget.ImageView;

import butterknife.BindView;
import butterknife.ButterKnife;
import rx.Single;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Func0;
import rx.schedulers.Schedulers;

public class MainActivity extends AppCompatActivity {
    /**
     * Load the native library on application startup
     */
    static {
        System.loadLibrary("imageproc-lib");
    }

    /**
     * Views that will be injected
     */
    @BindView(R.id.vg_root)
    ViewGroup mViewGroup;

    @BindView(R.id.iv_original)
    ImageView mIvOriginal;

    @BindView(R.id.iv_out1)
    ImageView mIvOut1;

    @BindView(R.id.iv_out2)
    ImageView mIvOut2;

    @BindView(R.id.iv_out3)
    ImageView mIvOut3;

    @BindView(R.id.iv_out4)
    ImageView mIvOut4;

    @BindView(R.id.iv_out5)
    ImageView mIvOut5;

    @BindView(R.id.iv_out6)
    ImageView mIvOut6;

    /**
     * Native methods that are implemented by the native library, which are packaged with the
     * application.
     */
    public native String naGetTitle();
    public native void naSetGaussianBlur(Bitmap bitmap, Bitmap output);
    public native void naSetBrightness(Bitmap bitmap, float brightnessValue);
    public native void naRemoveBlueColor(Bitmap bitmap);
    public native void naToBlackWhite(Bitmap bitmap);
    public native void naToGrayscale(Bitmap bitmap);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);

        setTitle(naGetTitle());

        final Bitmap src =
                BitmapFactory.decodeResource(getResources(), R.drawable.stef,
                        new BitmapFactory.Options());
        mIvOriginal.setImageBitmap(src);

        Single<Bitmap> b1 =
                Single.defer((Func0<Single<Bitmap>>) () ->
                        Single.create((Single.OnSubscribe<Bitmap>) singleSubscriber -> {
                            final Bitmap output = src.copy(Bitmap.Config.ARGB_8888, true);
                            naToGrayscale(output);
                            singleSubscriber.onSuccess(output);
                        })
                                .subscribeOn(Schedulers.computation())
                                .observeOn(AndroidSchedulers.mainThread())
                                .doOnSuccess(bitmap -> mIvOut1.setImageBitmap(bitmap)));

        Single<Bitmap> b2 =
                Single.defer((Func0<Single<Bitmap>>) () ->
                        Single.create((Single.OnSubscribe<Bitmap>) singleSubscriber -> {
                            final Bitmap output = src.copy(Bitmap.Config.ARGB_8888, true);
                            naToBlackWhite(output);
                            singleSubscriber.onSuccess(output);
                        })
                                .subscribeOn(Schedulers.computation())
                                .observeOn(AndroidSchedulers.mainThread())
                                .doOnSuccess(bitmap -> mIvOut2.setImageBitmap(bitmap)));

        Single<Bitmap> b3 =
                Single.defer((Func0<Single<Bitmap>>) () ->
                        Single.create((Single.OnSubscribe<Bitmap>) singleSubscriber -> {
                            final Bitmap output = src.copy(Bitmap.Config.ARGB_8888, true);
                            final Bitmap output2 = src.copy(Bitmap.Config.ARGB_8888, true);
                            naToGrayscale(output);
                            naSetBrightness(output, 2.0f);
                            naSetGaussianBlur(output, output2);
                            singleSubscriber.onSuccess(output2);
                        })
                                .subscribeOn(Schedulers.computation())
                                .observeOn(AndroidSchedulers.mainThread())
                                .doOnSuccess(bitmap -> mIvOut3.setImageBitmap(bitmap)));

        Single<Bitmap> b4 =
                Single.defer((Func0<Single<Bitmap>>) () ->
                        Single.create((Single.OnSubscribe<Bitmap>) singleSubscriber -> {
                            final Bitmap output = src.copy(Bitmap.Config.ARGB_8888, true);
                            naSetBrightness(output, 2.5f);
                            singleSubscriber.onSuccess(output);
                        })
                                .subscribeOn(Schedulers.computation())
                                .observeOn(AndroidSchedulers.mainThread())
                                .doOnSuccess(bitmap -> mIvOut4.setImageBitmap(bitmap)));

        Single<Bitmap> b5 =
                Single.defer((Func0<Single<Bitmap>>) () ->
                        Single.create((Single.OnSubscribe<Bitmap>) singleSubscriber -> {
                            final Bitmap output = src.copy(Bitmap.Config.ARGB_8888, true);
                            naSetGaussianBlur(src, output);
                            singleSubscriber.onSuccess(output);
                        })
                                .subscribeOn(Schedulers.computation())
                                .observeOn(AndroidSchedulers.mainThread())
                                .doOnSuccess(bitmap -> mIvOut5.setImageBitmap(bitmap)));

        Single<Bitmap> b6 =
                Single.defer((Func0<Single<Bitmap>>) () ->
                        Single.create((Single.OnSubscribe<Bitmap>) singleSubscriber -> {
                            final Bitmap output = src.copy(Bitmap.Config.ARGB_8888, true);
                            naRemoveBlueColor(output);
                            singleSubscriber.onSuccess(output);
                        })
                                .subscribeOn(Schedulers.computation())
                                .observeOn(AndroidSchedulers.mainThread())
                                .doOnSuccess(bitmap -> mIvOut6.setImageBitmap(bitmap)));

        Single.merge(b1, b2, b3, b4, b5, b6)
                .doOnSubscribe(() -> showMessage(getString(R.string.processing_started_message)))
                .doAfterTerminate(() -> showMessage(getString(R.string.processing_finished_message)))
                .subscribe();
    }

    private void showMessage(String message) {
        Snackbar.make(mViewGroup, message, Snackbar.LENGTH_SHORT).show();
    }
}
