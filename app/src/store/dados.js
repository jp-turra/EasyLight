
const state = {
  page: 'inicio',
  bluetoothState: false,
  device: [],
  userName: null
}

var mutations = {
  setUserName (state, name) {
    if (name) state.userName = name
  },
  setPage (state, payload) {
    if (payload) state.page = payload
  },
  setBluetoothState (state, payload) {
    if (payload) state.bluetoothState = payload
  },
  setDevice (state, payload) {
    if (payload) state.device = payload
  },
  addDevice (state, device) {
    if (device) state.device.push(device)
  }
}

const getters = {
  getUserName (state) {
    return state.userName
  },
  getPage (state) {
    return state.page
  },
  getBluetoothState (state) {
    return state.bluetoothState
  },
  getDevice (state) {
    return state.device
  }
}

export default {
  namespaced: false,
  getters,
  mutations,
  // actions,
  state
}
