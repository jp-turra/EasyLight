/* eslint-disable */
import bluetoothSerial from "app/src-cordova/plugins/cordova-plugin-bluetooth-serial/src/browser/bluetoothSerial";

export default ({ app, Vue }) => {
    Vue.mixin({
        abrirBluetooth() {
            bluetoothSerial.isEnabled(
                () => {
                    this.$store.commit("setBluetoothState", true);
                },
                () => {
                    if (this.$q.platform.is.android) {
                        bluetoothSerial.enable(
                            () => {
                                this.$store.commit("setBluetoothState", true);
                            },
                            () => {
                                this.$store.commit("setBluetoothState", false);
                                this.$store.commit("setPage", "pageOne");
                            }
                        );
                    } else {
                        Dialog.create({
                            title: "Atenção",
                            message: "Ative o seu bluetooth."
                        });
                        this.$store.commit("setPage", "pageOne");
                    }
                }
            );
            this.$store.commit("setPage", "bluetooth");
        }
    });
};
