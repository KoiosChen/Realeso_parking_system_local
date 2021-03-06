"""empty message

Revision ID: 023a84f760b0
Revises: ef96f8b5c87d
Create Date: 2018-08-12 22:54:14.708024

"""

# revision identifiers, used by Alembic.
revision = '023a84f760b0'
down_revision = 'ef96f8b5c87d'

from alembic import op
import sqlalchemy as sa


def upgrade():
    ### commands auto generated by Alembic - please adjust! ###
    op.add_column('parking_lot', sa.Column('effective_duration', sa.SmallInteger(), nullable=False))
    ### end Alembic commands ###


def downgrade():
    ### commands auto generated by Alembic - please adjust! ###
    op.drop_column('parking_lot', 'effective_duration')
    ### end Alembic commands ###
