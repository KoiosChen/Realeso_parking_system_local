//== Class definition

var DatatableRemoteAjaxDemo = function () {
    //== Private functions

    // basic demo
    var demo = function () {

        var datatable = $('#ajax_data').mDatatable({
            // datasource definition
            data: {
                type: 'remote',
                source: {
                    read: {
                        // sample GET method
                        method: 'POST',
                        url: '/syslog_search',
                        params: {
                            // custom query params
                            query: {
                                device_ip: $('#device_ip').val(),
                                logmsg: $('#logmsg').val(),
                                search_date: $('#m_daterangepicker_4 .form-control').val(),
                                serverty: $('#serverty').val()
                            }
                        },
                        map: function (raw) {
                            // sample data mapping
                            var dataSet = raw;
                            if (typeof raw.data !== 'undefined') {
                                dataSet = raw.data;
                            }
                            return dataSet;
                        },
                    },
                },
                pageSize: 10,
                serverPaging: true,
                serverFiltering: true,
                serverSorting: false,
            },

            // layout definition
            layout: {
                scroll: false,
                footer: false
            },

            // column sorting
            sortable: false,

            pagination: true,

            autoWidth: true,

            toolbar: {
                // toolbar items
                items: {
                    // pagination
                    pagination: {
                        // page size select
                        pageSizeSelect: [10, 20, 30, 50, 100],
                    },
                },
            },

            // columns definition

            columns: [
                {
                    field: 'id',
                    title: '#',
                    sortable: false, // disable sort for this column
                    //width: 60,
                    selector: false,
                    textAlign: 'center',
                }, {
                    field: 'device_name',
                    title: '设备名',
                    // sortable: 'asc', // default sort
                    filterable: false, // disable or enable filtering
                    //width: 100,
                    // basic templating support for column rendering,
                }, {
                    field: 'device_ip',
                    title: 'IP',
                    //width: 100,
                }, {
                    field: 'logmsg',
                    title: '日志信息',
                    //width: 400,
                }, {
                    field: 'serverty',
                    title: '级别',
                    //width: 60,
                }, {
                    field: 'logtime',
                    title: '日期',
                    type: 'date',
                    format: 'MM/DD/YYYY',
                }],
        });

    };

    return {
        // public functions
        init: function () {
            demo();
        },
    };
}();

$(document).ready(function () {
    DatatableRemoteAjaxDemo.init();
});

$('#submit_search').click(function () {
    $('#ajax_data').mDatatable().destroy();
    DatatableRemoteAjaxDemo.init();
})