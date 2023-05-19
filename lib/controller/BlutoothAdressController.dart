import 'dart:async';

import 'package:get/get.dart';

import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

class BlutoothAdressController extends GetxController {
  //static BlutoothAdressController get to => Get.find();

  BluetoothState bluetoothState = BluetoothState.UNKNOWN;

  String address = "...";
  String name = "...";

  Timer? discoverableTimeoutTimer;
  int discoverableTimeoutSecondsLeft = 0;
}
