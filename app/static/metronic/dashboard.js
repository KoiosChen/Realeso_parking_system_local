//== Daily Sales chart.
//** Based on Chartjs plugin - http://www.chartjs.org/
var syslogRanking = function () {
    var chartContainer = $('#m_chart_daily_sales');
    $('#div_alarm_ranking').showLoading()

    $.ajax({
        url: '/syslog_ranking',
        dataType: 'json',
        method: 'POST'
    }).done(function (results) {
        var chartData = {
            labels: results.labels,
            datasets: [{
                label: 'Error',
                backgroundColor: mUtil.getColor('success'),
                data: results.error_list
            }, {
                label: 'Critical',
                backgroundColor: mUtil.getColor('warning'),
                data: results.critical_list
            }, {
                label: 'Alert',
                backgroundColor: mUtil.getColor('brand'),
                data: results.alert_list
            }, {
                label: 'Emergency',
                backgroundColor: mUtil.getColor('danger'),
                data: results.emergency_list
            }]
        };

        var chart = new Chart(chartContainer, {
            type: 'bar',
            data: chartData,
            options: {
                title: {
                    display: false,
                },
                tooltips: {
                    intersect: false,
                    mode: 'nearest',
                    xPadding: 10,
                    yPadding: 10,
                    caretPadding: 10
                },
                legend: {
                    display: true,
                    position: 'left'
                },
                responsive: true,
                maintainAspectRatio: false,
                barRadius: 4,
                scales: {
                    xAxes: [{
                        display: false,
                        gridLines: false,
                        stacked: true
                    }],
                    yAxes: [{
                        display: false,
                        stacked: true,
                        gridLines: false
                    }]
                },
                layout: {
                    padding: {
                        left: 0,
                        right: 0,
                        top: 0,
                        bottom: 0
                    }
                }
            }
        });

        $("#div_alarm_ranking").hideLoading()
    });
}

//== Today's syslog counter
var todaySyslogCounter = function () {
    var syslogCounterSpan = $('#today_syslog_counter');
    var compareSpan = $('#syslog_counter_compare');

    syslogCounterSpan.showLoading();
    compareSpan.showLoading();

    $.ajax({
        url: '/today_syslog_counter',
        dataType: 'json',
        method: 'POST'
    }).done(function (results) {
        syslogCounterSpan.text(results.content);
        compareSpan.text(results.compare);
        syslogCounterSpan.hideLoading();
        compareSpan.hideLoading();
    })
}

//== Today's alarm counter
var todayAlarmCounter = function () {
    var alarmCounterSpan = $('#today_alarm_counter');
    alarmCounterSpan.showLoading();

    $.ajax({
        url: '/alarm_counter',
        dataType: 'json',
        method: 'POST'
    }).done(function (results) {
        alarmCounterSpan.text(results.content);
        alarmCounterSpan.hideLoading();
    })
}

//== The percent of wechat send successful
var todayWechatSendSuccess = function () {
    var wechatCounterSpan = $('#today_wechat_send_success');
    wechatCounterSpan.showLoading();

    $.ajax({
        url: '/wechat_counter',
        dataType: 'json',
        method: 'POST'
    }).done(function (results) {
        wechatCounterSpan.text(results.content);
        wechatCounterSpan.hideLoading();
    })
}

//== latest fifth alarms
var latestFifthAlarms = function () {
    $('#div_latest_update').showLoading()

    $.ajax({
        url: '/latest_fifth_alarms',
        dataType: 'json',
        method: 'POST'
    }).done(function (results) {
        if (results.status === 'Success') {
            for (var i = 0; i < results.content.length; i++) {
                $('#msg-' + String(i + 1)).text(results.content[i][0]);
                $('#msg-date-' + String(i + 1)).text(results.content[i][1]);
            }
            $('#div_latest_update').hideLoading()
        }
        else {
            $('#msg-1').text(results.content[0][0]);
            $('#msg-date-1').text(results.content[0][1]);
            $('#div_latest_update').hideLoading()
        }
    })
}

var realTimeSyslogRate = function () {
    myChart = echarts.init(document.getElementById("m_flotcharts_4"));
    $('#m_flotcharts_4').showLoading();
    // init data
    var date = [];
    var data = [];
    var pre_data = 0;
    var interval = 10000;
    var totalPoints = 10;
    for (var x = 0; x < totalPoints; ++x) {
        data.push(0)
    }

    for (var d = 0; d < totalPoints; ++d) {
        date.push(d + 1)
    }

    function addData(shift) {
        $.ajax({
            url: '/realTimeSyslogRate',
            data: {'pre_data': pre_data, 'interval': interval},
            dataType: 'json',
            method: 'POST'
        }).done(function (results) {
            pre_data = results.pre_data;
            data.push(results.content);
            $('#m_flotcharts_4').hideLoading();
        });

        if (shift) {
            data.shift();
        }

    }

    addData();

    option = {
        xAxis: {
            show: false,
            type: 'category',
            boundaryGap: false,
            data: date
        },
        yAxis: {
            boundaryGap: [0, '80%'],
            type: 'value'
        },
        series: [
            {
                name: '成交',
                type: 'line',
                smooth: true,
                symbol: 'none',
                stack: 'a',
                areaStyle: {
                    normal: {}
                },
                data: data
            }
        ]
    };

    setInterval(function () {
        addData(true);
        myChart.setOption(option);
    }, interval);
};


// using ECHARTS http://echarts.baidu.com/tutorial.html#个性化图表的样式
var keywordsRanking = function () {
    myChart2 = echarts.init(document.getElementById("m_chart_revenue_change"));
    $.ajax({
        url: '/keyAlarmRanking',
        dataType: 'json',
        method: 'POST'
    }).done(function (results) {
        if (results.status === 'Success') {
            myChart2.setOption({
                series: [
                    {
                        name: '关键字告警',
                        type: 'pie',
                        radius: '55%',
                        data: results.data
                    }
                ]
            });
        }
        else {
            var element = document.getElementById("m_chart_revenue_change");
            while (element.hasChildNodes()) //当elem下还存在子节点时 循环继续
            {
                element.removeChild(element.firstChild);
            }
            var para = document.createElement("div");
            para.setAttribute("class", "m-widget14__legend");

            var new_span1 = document.createElement("span");
            new_span1.setAttribute("class", "m-widget14__legend-bullet m--bg-danger");

            var new_span2 = document.createElement("span");
            new_span2.setAttribute("class", "m-widget14__legend-text");
            var node = document.createTextNode(results.content);
            new_span2.appendChild(node);

            para.appendChild(new_span1);
            para.appendChild(new_span2);


            element.appendChild(para)
            $('#keywords_ranking').hideLoading();
        }
    })
};


//== Class initialization on page load
jQuery(document).ready(function () {
    // init charts
    syslogRanking();

    todayWechatSendSuccess();

    // today syslog counter
    todaySyslogCounter();

    // today alarm counter
    todayAlarmCounter();

    // real time syslog receive rate
    realTimeSyslogRate();

    keywordsRanking();

    latestFifthAlarms();

    setInterval(function () {
        // init charts
        syslogRanking();

        todayWechatSendSuccess();

        // today syslog counter
        todaySyslogCounter();

        // today alarm counter
        todayAlarmCounter();

        keywordsRanking();

        latestFifthAlarms();
    }, 60000);

});