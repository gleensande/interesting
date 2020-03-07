import React, { PureComponent, Fragment } from 'react';

class Button extends PureComponent {
	constructor(props) {
		super(props);
		this.state = {
			className: props.className,
			innerText : props.innerText,
			handleClick : props.handleClick,
		};

		this.handleClick = this.handleClick.bind(this);
	}
	
	handleClick() {
		if (this.state.handleClick) {
			this.state.handleClick();
		}
	}

	componentDidUpdate(prevProps) {
		if (this.props.innerText !== prevProps.innerText) {
			this.setState({innerText : this.props.innerText});	
		}
	}

	render() {
		return (
			<Fragment>
				<div className = {this.state.className} onClick = {this.handleClick}>
					{this.state.innerText}
				</div>
			</Fragment>
    	);
  	}
}

export default Button;