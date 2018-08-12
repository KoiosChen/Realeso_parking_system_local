//== Class definition

var FormControls = function () {
    //== Private functions

    var demo1 = function () {
        $("#m_form_1").validate({
            // define validation rules
            rules: {
                username: {
                    required: true
                },
                password: {
                    required: true
                },
                password2: {
                    required: true,
                    equalTo: '#password'
                },
                phone: {
                    required: true,
                    phoneCN: true
                },
                role_select: {
                    required: true
                }
            },

            //display error alert on form submit
            invalidHandler: function (event, validator) {
                var alert = $('#m_form_1_msg');
                alert.removeClass('m--hide').show();
                mApp.scrollTo(alert, -200);
            },

            submitHandler: function (form) {
                let username = $('#username_new').val();
                let phone = $('#phone').val();
                let password = $('#password').val();
                let role_select = $('#role_select').val();
                let duty_select = $('#duty_select').val();

                let params = {
                    'username': username,
                    'phone': phone,
                    'password': password,
                    'role_select': role_select,
                    'duty_select': duty_select
                };
                $.ajax({
                    type: "POST",          //提交方式          
                    url: "user_register",  //提交的页面/方法名          
                    dataType: 'json',
                    contentType: "application/json; charset=utf-8",
                    data: JSON.stringify(params, null, '\t'),         //参数（如果没有参数：null）          

                    success: function (msg) {
                        $('#ajax_data').mDatatable().destroy();
                        DatatableRemoteAjaxDemo.init();
                        toastr.info(msg.content);
                        $('#user_register_model').modal('hide')
                    },
                    error: function (xhr, msg, e) {
                        toastr.warning(msg.content);
                        $('#user_register_model').modal('hide')
                    }
                });
            }
        });
    }

    var demo2 = function () {
        $("#m_form_update").validate({
            // define validation rules
            rules: {
                username_update: {
                    required: false
                },
                password_update: {
                    required: false
                },
                password2_update: {
                    required: false,
                    equalTo: '#password_update'
                },
                role_update: {
                    required: false,
                },
                duty_update: {
                    required: false,
                }
            },

            //display error alert on form submit
            invalidHandler: function (event, validator) {
                var alert = $('#m_form_update_msg');
                alert.removeClass('m--hide').show();
                mApp.scrollTo(alert, -200);
            },

            submitHandler: function (form) {
                let username_update = $('#username_update').val();
                let phone_update = $('#phone_update').val();
                let password_update = $('#password_update').val();
                let role_update = $('#role_update').val();
                let duty_update = $('#duty_update').val();

                let params = {
                    'phone_update': phone_update,
                    'username_update': username_update,
                    'password_update': password_update,
                    'role_update': role_update,
                    'duty_update': duty_update
                };
                $.ajax({
                    type: "POST",          //提交方式          
                    url: "user_update",  //提交的页面/方法名          
                    dataType: 'json',
                    contentType: "application/json; charset=utf-8",
                    data: JSON.stringify(params, null, '\t'),         //参数（如果没有参数：null）          

                    success: function (msg) {
                        if (msg.status === 'true') {
                            $('#ajax_data').mDatatable().destroy();
                            DatatableRemoteAjaxDemo.init();
                            toastr.info(msg.content);
                            $('#user_update_model').modal('hide')
                        }
                        else {
                            toastr.warn(msg.content);
                            $('#user_update_model').modal('hide')
                        }
                    },
                    error: function (xhr, msg, e) {
                        toastr.error(msg.content);
                        $('#user_update_model').modal('hide')
                    }
                });
            }
        });
    }


    return {
        // public functions
        init: function () {
            demo1();
            demo2();
        }
    };
}();

jQuery(document).ready(function () {
    FormControls.init();
});