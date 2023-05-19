import 'package:brace/screen/ChatPage.dart';
import 'package:brace/screen/DiscoveryPage.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key});

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  bool isDrawerOpen = true;

  void toggleDrawer() {
    setState(() {
      isDrawerOpen = !isDrawerOpen;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text("스마트 교정기 케이스"),
      ),
      body: Center(
          child: ListView(
        children: [
          Container(
            width: 300,
            height: 300,
            decoration: BoxDecoration(
              image: DecorationImage(
                image: isDrawerOpen
                    ? const AssetImage('assets/images/open.png')
                    : const AssetImage('assets/images/close.png'),
              ),
            ),
          ),
          ListTile(
            leading: const Icon(Icons.settings),
            title: const Text('교정기 설정'),
            onTap: () {
              Get.to(const DiscoveryPage());
            },
          ),
          ListTile(
            leading: const Icon(Icons.search),
            title: const Text('내 교정기 찾기'),
            onTap: () {},
          ),
          ListTile(
            leading: const Icon(Icons.alarm),
            title: const Text('팁과 사용 설명서'),
            onTap: () {},
          ),
          ListTile(
            leading: const Icon(Icons.chat),
            title: const Text('아두이노랑 채팅하기'),
            onTap: () {},
          ),
        ],
      )),
      floatingActionButton: FloatingActionButton(
        onPressed: () {
          toggleDrawer();
        },
        child: const Icon(Icons.change_circle),
      ),
    );
  }
}
