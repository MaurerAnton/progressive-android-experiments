package chat.progressive.app.features.testmode

import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import chat.progressive.app.R
import chat.progressive.app.native.ProgressiveNative
import org.json.JSONArray
import org.json.JSONObject

/**
 * Test Mode — explore app features without a Matrix account.
 * Uses C++ TestProvider for mock data.
 */
class TestModeActivity : AppCompatActivity() {

    private lateinit var roomList: RecyclerView
    private lateinit var messageList: RecyclerView
    private lateinit var searchInput: EditText
    private lateinit var statusText: TextView
    private lateinit var roomAdapter: RoomAdapter
    private lateinit var messageAdapter: MessageAdapter

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_test_mode)
        
        roomList = findViewById(R.id.testRoomList)
        messageList = findViewById(R.id.testMessageList)
        searchInput = findViewById(R.id.testSearchInput)
        statusText = findViewById(R.id.testStatus)
        
        roomAdapter = RoomAdapter { roomId -> loadMessages(roomId) }
        messageAdapter = MessageAdapter()
        
        roomList.layoutManager = LinearLayoutManager(this)
        roomList.adapter = roomAdapter
        messageList.layoutManager = LinearLayoutManager(this)
        messageList.adapter = messageAdapter
        
        findViewById<View>(R.id.testSearchBtn).setOnClickListener { doSearch() }
        
        loadRooms()
    }
    
    private fun loadRooms() {
        try {
            val json = ProgressiveNative.nativeTestGetRooms()
            val arr = JSONArray(json)
            val rooms = (0 until arr.length()).map { arr.getJSONObject(it) }
            roomAdapter.submitList(rooms)
            statusText.text = "${rooms.size} test rooms loaded"
        } catch (e: Exception) {
            statusText.text = "Error: ${e.message}"
        }
    }
    
    private fun loadMessages(roomId: String) {
        try {
            val json = ProgressiveNative.nativeTestGetMessages(roomId, 50, 0)
            val arr = JSONArray(json)
            val msgs = (0 until arr.length()).map { arr.getJSONObject(it) }
            messageAdapter.submitList(msgs)
            messageList.visibility = View.VISIBLE
            statusText.text = "${msgs.size} messages"
        } catch (e: Exception) {
            statusText.text = "Error: ${e.message}"
        }
    }
    
    private fun doSearch() {
        val term = searchInput.text.toString()
        if (term.isBlank()) return
        try {
            val json = ProgressiveNative.nativeTestSearch(term, "")
            val arr = JSONArray(json)
            val results = (0 until arr.length()).map { arr.getJSONObject(it) }
            messageAdapter.submitList(results)
            statusText.text = "Found ${results.size} messages for '$term'"
        } catch (e: Exception) {
            statusText.text = "Error: ${e.message}"
        }
    }
}

data class RoomItem(val id: String, val name: String, val topic: String, val unread: Int, val encrypted: Boolean)

class RoomAdapter(private val onClick: (String) -> Unit) : RecyclerView.Adapter<RoomAdapter.VH>() {
    private var items = listOf<JSONObject>()
    
    fun submitList(list: List<JSONObject>) { items = list; notifyDataSetChanged() }
    
    override fun onCreateViewHolder(parent: android.view.ViewGroup, viewType: Int) = VH(
        android.widget.TextView(parent.context).apply {
            setPadding(48, 24, 48, 24); textSize = 16f
            setOnClickListener { onClick(items[bindingAdapterPosition].getString("roomId")) }
        }
    )
    
    override fun onBindViewHolder(holder: VH, pos: Int) {
        val r = items[pos]
        val enc = if (r.optBoolean("isEncrypted")) " 🔒" else ""
        holder.v.text = "${r.getString("name")}$enc\n  ${r.getString("lastMessage")}"
    }
    
    override fun getItemCount() = items.size
    class VH(val v: TextView) : RecyclerView.ViewHolder(v)
}

class MessageAdapter : RecyclerView.Adapter<MessageAdapter.VH>() {
    private var items = listOf<JSONObject>()
    
    fun submitList(list: List<JSONObject>) { items = list; notifyDataSetChanged() }
    
    override fun onCreateViewHolder(parent: android.view.ViewGroup, viewType: Int) = VH(
        android.widget.TextView(parent.context).apply {
            setPadding(32, 16, 32, 16); textSize = 14f
        }
    )
    
    override fun onBindViewHolder(holder: VH, pos: Int) {
        val m = items[pos]
        holder.v.text = "${m.getString("senderName")}: ${m.getString("body")}"
    }
    
    override fun getItemCount() = items.size
    class VH(val v: TextView) : RecyclerView.ViewHolder(v)
}
