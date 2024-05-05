import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:fluttertoast/fluttertoast.dart';
import 'package:postgres/postgres.dart';

void main() async {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  @override
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
        debugShowCheckedModeBanner: false, home: HomPage());
  }
}

const textStyle = TextStyle(fontSize: 18);

class HomPage extends StatefulWidget {
  const HomPage({super.key});

  @override
  State<HomPage> createState() => _HomPageState();
}

class _HomPageState extends State<HomPage> {
  String host = '',
      port = '',
      username = '',
      password = '',
      databaseName = '',
      query = '';
  Connection? conn;
  List<String> resultsColumnNames = [];
  List<Map<String, dynamic>> resultsRows = [];
  final pageController = PageController();

  @override
  Widget build(BuildContext context) {
    var children = [
      Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          const Text("Запрос", style: textStyle),
          IconButton(
              onPressed: () => execute(context),
              icon: const Icon(Icons.play_arrow_rounded))
        ],
      ),
      TextFormField(
        onChanged: (value) => query = value,
        keyboardType: TextInputType.multiline,
        maxLines: null,
        autocorrect: false,
        decoration:
            InputDecoration(hintText: "Запрос", labelText: "Введите запрос"),
      ),
    ];

    if (!resultsRows.isEmpty) {
      children.add(
        Padding(
          padding: const EdgeInsets.all(10.0),
          child: SingleChildScrollView(
            scrollDirection: Axis.horizontal,
            child: DataTable(
              border: TableBorder.all(width: 1),
              columns: resultsColumnNames
                  .map((name) => DataColumn(
                        label: Text(name),
                      ))
                  .toList(),
              rows: resultsRows
                  .map((row) => DataRow(
                      cells: row.values
                          .map<DataCell>((value) => DataCell(
                              Text(value != null ? value.toString() : "NULL")))
                          .toList()))
                  .toList(),
            ),
          ),
        ),
      );
    }

    return Scaffold(
      appBar: AppBar(
        title: const Text('PostgreSQL Client'),
      ),
      body: PageView(
        controller: pageController,
        children: [
          SingleChildScrollView(
            child: Padding(
              padding: const EdgeInsets.all(10.0),
              child: Column(children: [
                Row(children: [
                  Flexible(
                      child: TextFormField(
                    onChanged: (value) => host = value,
                    decoration: InputDecoration(
                        border: const OutlineInputBorder(),
                        hintText: "Введите адрес",
                        labelText: "Адрес"),
                  )),
                  const SizedBox(width: 10),
                  Flexible(
                      child: TextFormField(
                    onChanged: (value) => port = value,
                    keyboardType: TextInputType.number,
                    inputFormatters: [
                      FilteringTextInputFormatter.digitsOnly,
                      LengthLimitingTextInputFormatter(4)
                    ],
                    decoration: InputDecoration(
                        border: const OutlineInputBorder(),
                        hintText: "Введите порт",
                        labelText: "Порт"),
                  ))
                ]),
                TextFormField(
                  onChanged: (value) => databaseName = value,
                  decoration: InputDecoration(
                      hintText: "Введите название базы данных",
                      labelText: "Название базы данных"),
                ),
                TextFormField(
                  onChanged: (value) => username = value,
                  decoration: InputDecoration(
                      hintText: "Имя пользователя",
                      labelText: "Введите имя пользователя"),
                ),
                TextFormField(
                  onChanged: (value) => password = value,
                  obscureText: true,
                  decoration: InputDecoration(
                      hintText: "Пароль", labelText: "Введите пароль"),
                ),
                const SizedBox(height: 15.0),
                FloatingActionButton.extended(
                    onPressed: connect, label: const Text("Подключиться"))
              ]),
            ),
          ),
          SingleChildScrollView(
            child: Padding(
              padding: const EdgeInsets.all(10.0),
              child: Column(
                children: [
                  Card(
                    child: Padding(
                      padding: const EdgeInsets.all(10.0),
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: children,
                      ),
                    ),
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }

  connect() async {
    if (host.isEmpty ||
        port.isEmpty ||
        databaseName.isEmpty ||
        username.isEmpty ||
        password.isEmpty) {
      toastMessage("Все поля должны быть заполнены", color: Colors.redAccent);

      return;
    }
    try {
      conn = await Connection.open(
        Endpoint(
          host: host,
          database: databaseName,
          username: username,
          password: password,
        ),
        settings: ConnectionSettings(
          sslMode: SslMode.disable,
        ),
      );

      toastMessage("Соединение установлено", color: Colors.green);
      pageController.nextPage(
          duration: const Duration(milliseconds: 500), curve: Curves.easeInOut);
    } catch (e) {
      toastMessage("Не удалось установить соединение", color: Colors.redAccent);
    }
  }

  Future<bool?> toastMessage(String message,
      {Color color = Colors.blueAccent}) {
    return Fluttertoast.showToast(
        msg: message,
        toastLength: Toast.LENGTH_SHORT,
        timeInSecForIosWeb: 1,
        backgroundColor: color,
        textColor: Colors.white,
        fontSize: 16.0);
  }

  execute(BuildContext context) async {
    if (query.isEmpty) {
      return;
    }
    final result = await conn?.execute(query);
    if (result == null || result.isEmpty) {
      return;
    }
    final rows = result?.map((row) => row.toColumnMap()).toList();
    if (rows == null || rows.isEmpty) {
      return;
    }
    setState(() {
      resultsColumnNames = rows[0].keys.toList();
      resultsRows = rows;
    });
  }
}
