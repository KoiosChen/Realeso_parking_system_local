"""empty message

Revision ID: 9ad7c6310a92
Revises: None
Create Date: 2018-07-15 15:55:07.585511

"""

# revision identifiers, used by Alembic.
revision = '9ad7c6310a92'
down_revision = None

from alembic import op
import sqlalchemy as sa


def upgrade():
    ### commands auto generated by Alembic - please adjust! ###
    op.create_table('api_configure',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('api_name', sa.String(length=20), nullable=False),
    sa.Column('api_params', sa.String(length=100), nullable=False),
    sa.Column('api_params_value', sa.String(length=200), nullable=True),
    sa.PrimaryKeyConstraint('id')
    )
    op.create_table('area',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('area_name', sa.String(length=30), nullable=False),
    sa.Column('area_desc', sa.String(length=200), nullable=True),
    sa.Column('area_parking_log', sa.String(length=200), nullable=True),
    sa.PrimaryKeyConstraint('id')
    )
    op.create_index(op.f('ix_area_area_name'), 'area', ['area_name'], unique=False)
    op.create_table('duty_attended_time',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('attended_time_name', sa.String(length=64), nullable=False),
    sa.Column('start_time', sa.Time(), nullable=False),
    sa.Column('stop_time', sa.Time(), nullable=False),
    sa.Column('day_adjust', sa.SmallInteger(), nullable=True),
    sa.Column('status', sa.SmallInteger(), nullable=False),
    sa.Column('create_time', sa.DateTime(), nullable=False),
    sa.PrimaryKeyConstraint('id')
    )
    op.create_table('duty_schedule',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('date_time', sa.Date(), nullable=True),
    sa.Column('userid', sa.String(length=100), nullable=False),
    sa.Column('attended_time_id', sa.SmallInteger(), nullable=False),
    sa.Column('duty_status', sa.SmallInteger(), nullable=False),
    sa.Column('priority', sa.SmallInteger(), nullable=False),
    sa.Column('create_time', sa.DateTime(), nullable=True),
    sa.PrimaryKeyConstraint('id')
    )
    op.create_table('job_desc',
    sa.Column('job_id', sa.SmallInteger(), nullable=False),
    sa.Column('job_name', sa.String(length=20), nullable=True),
    sa.Column('job_desc', sa.String(length=100), nullable=True),
    sa.Column('alarm_type', sa.String(length=20), nullable=True),
    sa.PrimaryKeyConstraint('job_id')
    )
    op.create_table('parking_lot_detail',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('name', sa.String(length=100), nullable=False),
    sa.Column('value', sa.String(length=100), nullable=False),
    sa.PrimaryKeyConstraint('id')
    )
    op.create_index(op.f('ix_parking_lot_detail_name'), 'parking_lot_detail', ['name'], unique=False)
    op.create_index(op.f('ix_parking_lot_detail_value'), 'parking_lot_detail', ['value'], unique=False)
    op.create_table('parking_orders',
    sa.Column('uuid', sa.String(length=50), nullable=False),
    sa.Column('number_plate', sa.String(length=10), nullable=False),
    sa.Column('order_type', sa.SmallInteger(), nullable=False),
    sa.Column('order_validate_start', sa.DateTime(), nullable=False),
    sa.Column('order_validate_stop', sa.DateTime(), nullable=False),
    sa.Column('reserved', sa.SmallInteger(), nullable=False),
    sa.Column('status', sa.SmallInteger(), nullable=True),
    sa.Column('create_time', sa.DateTime(), nullable=False),
    sa.Column('update_time', sa.DateTime(), nullable=False),
    sa.PrimaryKeyConstraint('uuid')
    )
    op.create_index(op.f('ix_parking_orders_create_time'), 'parking_orders', ['create_time'], unique=False)
    op.create_index(op.f('ix_parking_orders_number_plate'), 'parking_orders', ['number_plate'], unique=False)
    op.create_index(op.f('ix_parking_orders_order_type'), 'parking_orders', ['order_type'], unique=False)
    op.create_index(op.f('ix_parking_orders_order_validate_start'), 'parking_orders', ['order_validate_start'], unique=False)
    op.create_index(op.f('ix_parking_orders_order_validate_stop'), 'parking_orders', ['order_validate_stop'], unique=False)
    op.create_index(op.f('ix_parking_orders_reserved'), 'parking_orders', ['reserved'], unique=False)
    op.create_index(op.f('ix_parking_orders_status'), 'parking_orders', ['status'], unique=False)
    op.create_index(op.f('ix_parking_orders_update_time'), 'parking_orders', ['update_time'], unique=False)
    op.create_table('roles',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('name', sa.String(length=64), nullable=True),
    sa.Column('default', sa.Boolean(), nullable=True),
    sa.Column('permissions', sa.Integer(), nullable=True),
    sa.PrimaryKeyConstraint('id'),
    sa.UniqueConstraint('name')
    )
    op.create_index(op.f('ix_roles_default'), 'roles', ['default'], unique=False)
    op.create_table('token_record',
    sa.Column('unique_id', sa.String(length=128), nullable=False),
    sa.Column('token', sa.String(length=512), nullable=False),
    sa.Column('expire', sa.String(length=10), nullable=True),
    sa.Column('create_time', sa.DateTime(), nullable=True),
    sa.PrimaryKeyConstraint('unique_id')
    )
    op.create_table('parking_lot',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('name', sa.String(length=24), nullable=False),
    sa.Column('address', sa.String(length=100), nullable=False),
    sa.Column('floors', sa.SmallInteger(), nullable=True),
    sa.Column('floors_desc', sa.String(length=200), nullable=True),
    sa.Column('parking_space_totally', sa.Integer(), nullable=False),
    sa.Column('parking_space_fixed', sa.Integer(), nullable=False),
    sa.Column('free_minutes', sa.SmallInteger(), nullable=False),
    sa.Column('start_minutes', sa.SmallInteger(), nullable=False),
    sa.Column('pay_interval', sa.SmallInteger(), nullable=False),
    sa.Column('status', sa.Integer(), nullable=False),
    sa.Column('permit_value', sa.String(length=200), nullable=True),
    sa.Column('parking_lot_detail_id', sa.Integer(), nullable=True),
    sa.ForeignKeyConstraint(['parking_lot_detail_id'], ['parking_lot_detail.id'], ),
    sa.PrimaryKeyConstraint('id'),
    sa.UniqueConstraint('address'),
    sa.UniqueConstraint('name')
    )
    op.create_table('users',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('email', sa.String(length=64), nullable=True),
    sa.Column('phoneNum', sa.String(length=15), nullable=True),
    sa.Column('username', sa.String(length=64), nullable=True),
    sa.Column('role_id', sa.Integer(), nullable=True),
    sa.Column('area', sa.Integer(), nullable=True),
    sa.Column('duty', sa.Integer(), nullable=True),
    sa.Column('permit_machine_room', sa.String(length=200), nullable=True),
    sa.Column('password_hash', sa.String(length=128), nullable=True),
    sa.Column('status', sa.SmallInteger(), nullable=True),
    sa.ForeignKeyConstraint(['role_id'], ['roles.id'], ),
    sa.PrimaryKeyConstraint('id'),
    sa.UniqueConstraint('phoneNum')
    )
    op.create_index(op.f('ix_users_email'), 'users', ['email'], unique=True)
    op.create_index(op.f('ix_users_username'), 'users', ['username'], unique=False)
    op.create_table('camera',
    sa.Column('device_number', sa.String(length=100), nullable=False),
    sa.Column('device_name', sa.String(length=200), nullable=False),
    sa.Column('device_type', sa.SmallInteger(), nullable=True),
    sa.Column('parking_lot_id', sa.Integer(), nullable=True),
    sa.Column('status', sa.Integer(), nullable=False),
    sa.Column('vendor', sa.String(length=100), nullable=True),
    sa.Column('gate_id', sa.String(length=100), nullable=True),
    sa.Column('description', sa.String(length=200), nullable=True),
    sa.ForeignKeyConstraint(['parking_lot_id'], ['parking_lot.id'], ),
    sa.PrimaryKeyConstraint('device_number'),
    sa.UniqueConstraint('device_name')
    )
    op.create_table('fixed_parking_space',
    sa.Column('uuid', sa.String(length=50), nullable=False),
    sa.Column('number_plate', sa.String(length=20), nullable=False),
    sa.Column('specified_parking_space_code', sa.String(length=10), nullable=True),
    sa.Column('company', sa.String(length=100), nullable=True),
    sa.Column('room', sa.String(length=20), nullable=True),
    sa.Column('order_validate_start', sa.DateTime(), nullable=False),
    sa.Column('order_validate_stop', sa.DateTime(), nullable=False),
    sa.Column('registrar_id', sa.Integer(), nullable=True),
    sa.Column('create_time', sa.DateTime(), nullable=True),
    sa.ForeignKeyConstraint(['registrar_id'], ['users.id'], ),
    sa.PrimaryKeyConstraint('uuid')
    )
    op.create_index(op.f('ix_fixed_parking_space_number_plate'), 'fixed_parking_space', ['number_plate'], unique=False)
    op.create_index(op.f('ix_fixed_parking_space_order_validate_start'), 'fixed_parking_space', ['order_validate_start'], unique=False)
    op.create_index(op.f('ix_fixed_parking_space_order_validate_stop'), 'fixed_parking_space', ['order_validate_stop'], unique=False)
    op.create_table('parking_records',
    sa.Column('uuid', sa.String(length=50), nullable=False),
    sa.Column('number_plate', sa.String(length=10), nullable=False),
    sa.Column('entry_time', sa.DateTime(), nullable=False),
    sa.Column('entry_camera_id', sa.String(length=100), nullable=True),
    sa.Column('entry_pic', sa.String(length=100), nullable=True),
    sa.Column('entry_unit_price', sa.Float(), nullable=False),
    sa.Column('exit_time', sa.DateTime(), nullable=True),
    sa.Column('exit_pic', sa.String(length=100), nullable=True),
    sa.Column('exit_camera_id', sa.String(length=100), nullable=True),
    sa.Column('cashier_id', sa.Integer(), nullable=True),
    sa.Column('fee', sa.Float(), nullable=True),
    sa.Column('create_time', sa.DateTime(), nullable=True),
    sa.ForeignKeyConstraint(['cashier_id'], ['users.id'], ),
    sa.ForeignKeyConstraint(['entry_camera_id'], ['camera.device_number'], ),
    sa.ForeignKeyConstraint(['exit_camera_id'], ['camera.device_number'], ),
    sa.PrimaryKeyConstraint('uuid')
    )
    op.create_index(op.f('ix_parking_records_entry_pic'), 'parking_records', ['entry_pic'], unique=False)
    op.create_index(op.f('ix_parking_records_entry_time'), 'parking_records', ['entry_time'], unique=False)
    op.create_index(op.f('ix_parking_records_entry_unit_price'), 'parking_records', ['entry_unit_price'], unique=False)
    op.create_index(op.f('ix_parking_records_exit_pic'), 'parking_records', ['exit_pic'], unique=False)
    op.create_index(op.f('ix_parking_records_exit_time'), 'parking_records', ['exit_time'], unique=False)
    op.create_index(op.f('ix_parking_records_number_plate'), 'parking_records', ['number_plate'], unique=False)
    op.create_table('order_and_records',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('record_id', sa.String(length=50), nullable=True),
    sa.Column('order_id', sa.String(length=50), nullable=True),
    sa.ForeignKeyConstraint(['order_id'], ['parking_orders.uuid'], ),
    sa.ForeignKeyConstraint(['record_id'], ['parking_records.uuid'], ),
    sa.PrimaryKeyConstraint('id')
    )
    op.create_index(op.f('ix_order_and_records_order_id'), 'order_and_records', ['order_id'], unique=False)
    op.create_index(op.f('ix_order_and_records_record_id'), 'order_and_records', ['record_id'], unique=False)
    ### end Alembic commands ###


def downgrade():
    ### commands auto generated by Alembic - please adjust! ###
    op.drop_index(op.f('ix_order_and_records_record_id'), table_name='order_and_records')
    op.drop_index(op.f('ix_order_and_records_order_id'), table_name='order_and_records')
    op.drop_table('order_and_records')
    op.drop_index(op.f('ix_parking_records_number_plate'), table_name='parking_records')
    op.drop_index(op.f('ix_parking_records_exit_time'), table_name='parking_records')
    op.drop_index(op.f('ix_parking_records_exit_pic'), table_name='parking_records')
    op.drop_index(op.f('ix_parking_records_entry_unit_price'), table_name='parking_records')
    op.drop_index(op.f('ix_parking_records_entry_time'), table_name='parking_records')
    op.drop_index(op.f('ix_parking_records_entry_pic'), table_name='parking_records')
    op.drop_table('parking_records')
    op.drop_index(op.f('ix_fixed_parking_space_order_validate_stop'), table_name='fixed_parking_space')
    op.drop_index(op.f('ix_fixed_parking_space_order_validate_start'), table_name='fixed_parking_space')
    op.drop_index(op.f('ix_fixed_parking_space_number_plate'), table_name='fixed_parking_space')
    op.drop_table('fixed_parking_space')
    op.drop_table('camera')
    op.drop_index(op.f('ix_users_username'), table_name='users')
    op.drop_index(op.f('ix_users_email'), table_name='users')
    op.drop_table('users')
    op.drop_table('parking_lot')
    op.drop_table('token_record')
    op.drop_index(op.f('ix_roles_default'), table_name='roles')
    op.drop_table('roles')
    op.drop_index(op.f('ix_parking_orders_update_time'), table_name='parking_orders')
    op.drop_index(op.f('ix_parking_orders_status'), table_name='parking_orders')
    op.drop_index(op.f('ix_parking_orders_reserved'), table_name='parking_orders')
    op.drop_index(op.f('ix_parking_orders_order_validate_stop'), table_name='parking_orders')
    op.drop_index(op.f('ix_parking_orders_order_validate_start'), table_name='parking_orders')
    op.drop_index(op.f('ix_parking_orders_order_type'), table_name='parking_orders')
    op.drop_index(op.f('ix_parking_orders_number_plate'), table_name='parking_orders')
    op.drop_index(op.f('ix_parking_orders_create_time'), table_name='parking_orders')
    op.drop_table('parking_orders')
    op.drop_index(op.f('ix_parking_lot_detail_value'), table_name='parking_lot_detail')
    op.drop_index(op.f('ix_parking_lot_detail_name'), table_name='parking_lot_detail')
    op.drop_table('parking_lot_detail')
    op.drop_table('job_desc')
    op.drop_table('duty_schedule')
    op.drop_table('duty_attended_time')
    op.drop_index(op.f('ix_area_area_name'), table_name='area')
    op.drop_table('area')
    op.drop_table('api_configure')
    ### end Alembic commands ###
