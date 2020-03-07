import React, { PureComponent, Fragment } from 'react';

class Task extends PureComponent {
	constructor(props) {
		super(props);
		this.state = {
			id : props.id,
			innerText: props.innerText,
			handleClick: props.handleClick,
		};
	}

	render() {
		return (
			<Fragment>
				<div className = "task">
					{this.state.innerText}
				</div>
			</Fragment>
		);
	}
}

export default Task;
