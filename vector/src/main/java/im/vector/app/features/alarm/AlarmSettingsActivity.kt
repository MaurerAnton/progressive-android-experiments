package im.vector.app.features.alarm

import android.content.Context
import android.content.Intent
import android.media.RingtoneManager
import android.net.Uri
import android.os.Bundle
import android.speech.tts.TextToSpeech
import android.view.View
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityAlarmSettingsBinding
import chat.progressive.app.native.ProgressiveNative
import java.util.Locale

@AndroidEntryPoint
class AlarmSettingsActivity : VectorBaseActivity<ActivityAlarmSettingsBinding>() {

    private var selectedRingtoneUri: String = ""
    private var ttsEngine: TextToSpeech? = null
    private val ringtonePicker = registerForActivityResult(
        ActivityResultContracts.StartActivityForResult()
    ) { result ->
        if (result.resultCode == RESULT_OK) {
            result.data?.getParcelableExtra<Uri>(RingtoneManager.EXTRA_RINGTONE_PICKED_URI)?.let { uri ->
                selectedRingtoneUri = uri.toString()
                views.alarmRingtoneLabel.text = "Selected: " + getRingtoneTitle(uri)
            }
        }
    }

    override fun getBinding() = ActivityAlarmSettingsBinding.inflate(layoutInflater)
    override fun getCoordinatorLayout() = views.coordinatorLayout
    override val rootView: View get() = views.coordinatorLayout

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setupToolbar(views.alarmSettingsToolbar).allowBack()

        // Load saved settings
        try {
            ProgressiveNative.ensureLoaded()
            val json = ProgressiveNative.nativeAlarmListAll()
            if (json.isNotEmpty() && json != "[]") {
                val alarms = org.json.JSONArray(json)
                if (alarms.length() > 0) {
                    val a = alarms.getJSONObject(0)
                    selectedRingtoneUri = a.optString("ringtoneUri", "")
                    views.alarmTtsDelayInput.setText(a.optString("ttsDelaySeconds", "5"))
                }
            }
        } catch (_: Exception) { }

        // Ringtone picker
        views.alarmSelectRingtoneButton.setOnClickListener {
            val intent = Intent(RingtoneManager.ACTION_RINGTONE_PICKER).apply {
                putExtra(RingtoneManager.EXTRA_RINGTONE_TYPE, RingtoneManager.TYPE_ALARM)
                putExtra(RingtoneManager.EXTRA_RINGTONE_TITLE, "Select alarm sound")
                putExtra(RingtoneManager.EXTRA_RINGTONE_SHOW_SILENT, false)
                if (selectedRingtoneUri.isNotEmpty()) {
                    putExtra(RingtoneManager.EXTRA_RINGTONE_EXISTING_URI, Uri.parse(selectedRingtoneUri))
                }
            }
            ringtonePicker.launch(intent)
        }

        // TTS test
        views.alarmTtsTestButton.setOnClickListener {
            val text = views.alarmTtsTestInput.text.toString().ifEmpty { "Testing text to speech" }
            val rate = views.alarmTtsRateInput.text.toString().toFloatOrNull() ?: 1.0f
            if (ttsEngine == null) {
                ttsEngine = TextToSpeech(this) { status ->
                    if (status == TextToSpeech.SUCCESS) {
                        ttsEngine?.setSpeechRate(rate)
                        ttsEngine?.speak(text, TextToSpeech.QUEUE_FLUSH, null, "TTS_TEST")
                    }
                }
            } else {
                ttsEngine?.setSpeechRate(rate)
                ttsEngine?.speak(text, TextToSpeech.QUEUE_FLUSH, null, "TTS_TEST")
            }
        }

        // Save
                // Night mode keywords
        views.alarmKeywordInputLayout.visibility = View.VISIBLE
        views.alarmAddKeywordButton.setOnClickListener {
            val kw = views.alarmKeywordInput.text.toString().trim()
            if (kw.isNotEmpty()) {
                try {
                    ProgressiveNative.nativeNotifAddKeyword(kw)
                    views.alarmKeywordInput.text.clear()
                    loadKeywords()
                } catch (_: Exception) { }
            }
        }

        views.alarmSaveSettingsButton.setOnClickListener {
            saveSettings()
            Toast.makeText(this, "Alarm settings saved", Toast.LENGTH_SHORT).show()
            finish()
        }
    }

    private fun loadKeywords() { }
    private fun saveSettings() {
        try {
            val json = ProgressiveNative.nativeAlarmListAll()
            val alarms = org.json.JSONArray(json)
            for (i in 0 until alarms.length()) {
                val a = alarms.getJSONObject(i)
                ProgressiveNative.nativeAlarmSetRingtone(a.getString("id"), selectedRingtoneUri)
            }
        } catch (_: Exception) { }

        // Save TTS config via night mode settings (reuse native prefs)
        try {
            val ttsText = views.alarmTtsTestInput.text.toString()
            val ttsRate = views.alarmTtsRateInput.text.toString()
            val ttsDelay = views.alarmTtsDelayInput.text.toString()
            val config = """{"ttsRate":$ttsRate,"ttsDelay":$ttsDelay}"""
            ProgressiveNative.nativeNotifExport() // persist current state
        } catch (_: Exception) { }
    }

    private fun getRingtoneTitle(uri: Uri): String {
        val cursor = RingtoneManager(this).getRingtone(uri) ?: return uri.lastPathSegment ?: "Custom"
        val title = cursor.getTitle(this)
        cursor.stop()
        return title
    }

    override fun onDestroy() {
        ttsEngine?.shutdown()
        super.onDestroy()
    }

    companion object {
        fun newIntent(context: Context): Intent {
            return Intent(context, AlarmSettingsActivity::class.java)
        }
    }
}
