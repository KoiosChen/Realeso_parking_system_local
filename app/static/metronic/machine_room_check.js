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
                        url: '/machine_room',
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
                    selector: false,
                    textAlign: 'center',
                }, {
                    field: 'machine_room_name',
                    title: '机房名称',
                    textAlign: 'center',
                    // sortable: 'asc', // default sort
                    filterable: false, // disable or enable filtering
                    // basic templating support for column rendering,
                }, {
                    field: 'machine_room_address',
                    title: '机房地址',
                    textAlign: 'center',
                }, {
                    field: 'machine_room_level',
                    title: '机房级别',
                    textAlign: 'center',
                }, {
                    field: 'machine_room_status',
                    title: '机房状态',
                    textAlign: 'center',
                }, {
                    field: 'Actions',
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