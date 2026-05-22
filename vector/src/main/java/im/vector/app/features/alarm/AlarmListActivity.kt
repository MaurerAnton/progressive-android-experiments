package im.vector.app.features.alarm

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.LinearLayout
import android.widget.TextView
import androidx.core.content.ContextCompat
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.R
import im.vector.app.core.platform.ProgressiveActivity
import im.vector.app.databinding.ActivityAlarmListBinding
import chat.progressive.app.native.ProgressiveNative
import org.json.JSONArray
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

@AndroidEntryPoint
class AlarmListActivity : ProgressiveActivity<ActivityAlarmListBinding>() {

    override fun getBinding() = ActivityAlarmListBinding.inflate(layoutInflater)
    override fun getCoordinatorLayout() = views.coordinatorLayout
    override val rootView: View get() = views.coordinatorLayout

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setupToolbar(views.alarmListToolbar).allowBack()
        views.alarmListToolbar.menu.add(0, 1, 0, "Settings").setShowAsAction(1)
        views.alarmListToolbar.setOnMenuItemClickListener { item ->
            if (item.itemId == 1) startActivity(AlarmSettingsActivity.newIntent(this))
            true
        }
        loadAlarms()
    }

    private fun loadAlarms() {
        views.alarmListContainer.removeAllViews()
        try {
            ProgressiveNative.ensureLoaded()
            val json = ProgressiveNative.nativeAlarmListAll()
            val alarms = JSONArray(json)
            val sdf = SimpleDateFormat("MMM dd, HH:mm", Locale.getDefault())

            for (i in 0 until alarms.length()) {
                val a = alarms.getJSONObject(i)
                val note = a.getString("note")
                val triggerMs = a.getLong("triggerAtMs")
                val enabled = a.getBoolean("enabled")
                val id = a.getString("id")

                val item = layoutInflater.inflate(R.layout.item_alarm, views.alarmListContainer, false)
                val noteView: TextView = item.findViewById(R.id.alarmItemNote)
                val timeView: TextView = item.findViewById(R.id.alarmItemTime)
                val deleteBtn: View = item.findViewById(R.id.alarmItemDelete)

                noteView.text = note
                timeView.text = if (triggerMs > 0) sdf.format(Date(triggerMs)) else "Now"
                if (!enabled) noteView.alpha = 0.5f

                deleteBtn.setOnClickListener {
                    ProgressiveNative.nativeAlarmDelete(id)
                    loadAlarms()
                }

                item.findViewById<android.widget.TextView>(R.id.alarmItemDelete + 1)?.setOnClickListener {
                    try {
                        val shareIntent = android.content.Intent(android.content.Intent.ACTION_SEND).apply {
                            type = "text/plain"
                            putExtra(android.content.Intent.EXTRA_TEXT, note)
                        }
                        startActivity(android.content.Intent.createChooser(shareIntent, "Share alarm"))
                    } catch (_: Exception) { }
                }
                item.setOnClickListener {
                    val trigger = a.optLong("triggerAtMs", 0)
                    val ttsDelay = a.optInt("ttsDelaySeconds", 5)
                    val preAction = a.optString("preActionParam", "").ifEmpty { null }
                    startActivity(AlarmActivity.newIntent(this, id, note, ttsDelay, preAction))
                }

                views.alarmListContainer.addView(item)
            }
        } catch (_: Exception) { }
    }

    companion object {
        fun newIntent(context: Context): Intent {
            return Intent(context, AlarmListActivity::class.java)
        }
    }
}
