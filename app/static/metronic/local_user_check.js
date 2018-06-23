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
                        url: '/local_user_check',
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
                    textAlign: 'center'
                }, {
                    field: 'email',
                    title: '邮箱',
                    textAlign: 'center',
                    width: 250
                    // basic templating support for column rendering,
                }, {
                    field: 'username',
                    title: '用户名',
                    textAlign: 'center',
                    width: 105
                }, {
                    field: 'phoneNum',
                    title: '电话',
                    textAlign: 'center',
                    width: 88
                }, {
                    field: 'area',
                    title: '区域',
                    textAlign: 'center',
                    width: 50
                }, {
                    field: 'role',
                    title: '权限',
                    textAlign: 'center',
                    width: 110
                }, {
                    field: 'Actions',
                    width: 100,
                    textAlign: 'center',
                    title: '操作',
                    sortable: false,
                    overflow: 'visible',
                    template: function (row, index, datatable) {
                        return '<a ' + 'data-toggle="modal" data-target="#update" onclick="editInfo(' +  row.id + ', \'' + row.username + '\', \'' + row.phoneNum + '\', \'' + row.email + '\')" class="m-portlet__nav-link btn m-btn m-btn--hover-accent m-btn--icon m-btn--icon-only m-btn--pill" title="Edit details">\
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