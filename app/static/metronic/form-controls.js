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
                email: {
                    required: true,
                    email: true,
                    minlength: 10
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
                },
                area_select: {
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
                var this_modal = $('#user_register_model')
                var username = $('#username_new').val();
                var email = $('#email').val();
                var phone = $('#phone').val();
                var password = $('#password').val();
                var role_select = $('#role_select').val();
                var duty_select = $('#duty_select').val();
                var machine_room_select = $('#machine_room_select').val();
                var area_select = $('#area_select').val();

                var params = {
                    'username': username,
                    'email': email,
                    'phone': phone,
                    'password': password,
                    'role_select': role_select,
                    'duty_select': duty_select,
                    'area_select': area_select,
                    'machine_room_select': machine_room_select
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



    return {
        // public functions
        init: function () {
            demo1();
        }
    };
}();

jQuery(document).ready(function () {
    FormControls.init();
});