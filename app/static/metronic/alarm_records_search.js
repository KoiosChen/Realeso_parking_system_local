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
                        url: '/alarm_record',
                        params: {
                            // custom query params
                            query: {
                                select_handler: $('#select_handler').val(),
                                search_content: $('#search_content').val(),
                                search_date: $('#m_daterangepicker_4 .form-control').val(),
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
                    //width: 50,
                    selector: false,
                    textAlign: 'center',
                }, {
                    field: 'alarm_content',
                    title: '告警内容',
                    textAlign: 'center',
                    // sortable: 'asc', // default sort
                    filterable: false, // disable or enable filtering
                    //width: 650,
                    // basic templating support for column rendering,
                }, {
                    field: 'handler',
                    textAlign: 'center',
                    title: '处理人',
                    //width: 60,
                }, {
                    field: 'handle_time',
                    textAlign: 'center',
                    title: '处理时间',
                    //width: 150,
                }, {
                    field: 'alarm_time',
                    textAlign: 'center',
                    title: '告警时间',
                    //width: 150,
                    type: 'date',
                    format: 'MM/DD/YYYY',
                }, {
                    field: 'Actions',
                    //width: 110,
                    textAlign: 'center',
                    title: '操作',
                    sortable: false,
                    overflow: 'visible',
                    template: function (row, index, datatable) {
                        return '<a data-toggle="modal" data-target="#attachment" onclick="attachmentInfo(' + row.id + ')" class="m-portlet__nav-link btn m-btn m-btn--hover-accent m-btn--icon m-btn--icon-only m-btn--pill" title="Edit details">\
                                    <i class="la la-ellipsis-h"></i>\
                                </a>\
                                <a data-toggle="modal" data-target="#update" onclick="editInfo(' + row.id + ')" class="m-portlet__nav-link btn m-btn m-btn--hover-accent m-btn--icon m-btn--icon-only m-btn--pill" title="Edit details">\
                                    <i class="la la-edit"></i>\
                                </a>\
                                <a ' + 'onClick="return HTMerDel(' + row.id + ')" class="m-portlet__nav-link btn m-btn m-btn--hover-danger m-btn--icon m-btn--icon-only m-btn--pill" title="Delete">\
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