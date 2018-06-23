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
                        url: '/syslog_config',
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
                    width: 20,
                    selector: false,
                    textAlign: 'center',
                }, {
                    field: 'alarm_type',
                    title: '类型',
                    textAlign: 'center',
                    // sortable: 'asc', // default sort
                    filterable: false, // disable or enable filtering
                    width: 60,
                    // basic templating support for column rendering,
                }, {
                    field: 'alarm_level',
                    title: '级别',
                    textAlign: 'center',
                    width: 30,
                }, {
                    field: 'alarm_name',
                    title: '别名',
                    textAlign: 'center',
                    width: 150,
                }, {
                    field: 'alarm_keyword',
                    title: '匹配关键字',
                    textAlign: 'center',
                    width: 300,
                }, {
                    field: 'alarm_status',
                    title: '状态',
                    textAlign: 'center',
                    width: 30,
                }, {
                    field: 'alarm_create_time',
                    title: '日期',
                    type: 'date',
                    textAlign: 'center',
                    format: 'MM/DD/YYYY',
                }, {
                    field: 'Actions',
                    width: 110,
                    textAlign: 'center',
                    title: '操作',
                    sortable: false,
                    overflow: 'visible',
                    template: function (row, index, datatable) {
                        return '<a ' + 'onClick="return HTMerDel(' + row.id + ')" class="m-portlet__nav-link btn m-btn m-btn--hover-danger m-btn--icon m-btn--icon-only m-btn--pill" title="Delete">\
                                    <i class="la la-trash"></i>\
                                </a>\
                            ';
                    },
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

jQuery(document).ready(function () {
    DatatableRemoteAjaxDemo.init();
});

$('#submit_search').click(function () {
    $('#ajax_data').mDatatable().destroy();
    DatatableRemoteAjaxDemo.init();
})